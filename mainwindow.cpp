#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogo_cargar.h"
#include "guardado.h"
#include <QDir>
#include <QMessageBox>
#include <QPushButton>
#include "configuracion.h"
#include "lectorconfiguracion.h"
#include <QCoreApplication>
#include <QDebug>

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized();
    this->accedePartida = new partida(this);
    ui->IniciaPartida->setEnabled(0);
    this->reglas = new class reglas(this);

    const string ruta = (QCoreApplication::applicationDirPath() + "/configuracion.txt").toStdString();
    Configuracion config(this->juego.getTabla_danio(), ui->cantidad_cartas->value());
    LectorConfiguracion lector;
    if(!lector.leer(ruta, config)){
        lector.escribir_archivo(ruta, config);
    }

    qDebug() << QString::fromStdString(lector.mensaje_error());

    this->juego.aplicar_configuracion(config);
    this->accedePartida->aplicar_configuracion(config);

    ui->cantidad_cartas->setValue(config.get_cantidad());
    if(config.tiene_tipo_ia()){
        ui->elegir_IA->setCurrentIndex(config.get_tipo_ia());
    }

    // Botón "cargar_partida": se usa el del .ui si existe; si no, se crea uno
    // debajo del de iniciar partida, con el mismo estilo.
    QPushButton *boton_cargar = this->findChild<QPushButton *>("cargar_partida");
    if (!boton_cargar) {
        boton_cargar = new QPushButton("Cargar partida", ui->centralwidget);
        boton_cargar->setObjectName("cargar_partida");
        boton_cargar->setStyleSheet(ui->IniciaPartida->styleSheet());
        boton_cargar->setFont(ui->IniciaPartida->font());
        boton_cargar->setMinimumHeight(60);
        ui->gridLayout_2->addWidget(boton_cargar, 22, 1, 1, 7);
    }
    connect(boton_cargar, &QPushButton::clicked, this, &MainWindow::cargar_partida_guardada);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_elegir_IA_currentIndexChanged(int index)
{
    this->index=index;
    if(index>0){
    this->ui->IniciaPartida->setEnabled(1);
    }
    else this->ui->IniciaPartida->setEnabled(0);
}

void MainWindow::on_IniciaPartida_clicked()
{
    iniciar_partida();
}

void MainWindow::iniciar_partida()
{
    this->accedePartida->setDificultad_ia(this->index);
    this->accedePartida->setCant_cartas(this->ui->cantidad_cartas->value());
    this->accedePartida->setNombre(this->ui->caja_nombre->toPlainText());
    this->accedePartida->asignar_cartas();  
    this->accedePartida->showMaximized();
    this->hide();
}



void MainWindow::cargar_partida_guardada()
{
    int danadas = 0;
    const std::vector<partida_guardada> partidas = guardado::listar(&danadas);

    if (partidas.empty()) {
        QString texto = "Todavía no hay partidas guardadas.\n\nCarpeta de partidas:\n" +
                        QDir::toNativeSeparators(guardado::carpeta());
        if (danadas > 0)
            texto += "\n\nNo se pudieron leer " + QString::number(danadas) +
                     " archivo(s) de esa carpeta (dañados o de otra versión).";
        QMessageBox::information(this, "Cargar partida", texto);
        return;
    }

    dialogo_cargar dialogo(partidas, danadas, this);
    if (dialogo.exec() != QDialog::Accepted)
        return;

    const int fila = dialogo.elegida();
    if (fila < 0 || fila >= static_cast<int>(partidas.size()))
        return;

    this->accedePartida->cargar_partida(partidas[fila].datos);
    this->accedePartida->showMaximized();
    this->hide();
}



void MainWindow::on_tabla_danio_boton_clicked()
{
    // Se crea una sola vez (con parent, Qt la libera) en lugar de una ventana nueva por clic
    if (!this->tabla)
        this->tabla = new tabla_danio_ventana(&this->juego, this);
    this->tabla->show();
}


void MainWindow::on_reglas_clicked()
{
    this->reglas->show();
}

