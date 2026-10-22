#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogo_cargar.h"
#include "guardado.h"
#include <QMessageBox>
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



void MainWindow::on_cargar_partida_clicked()
{
    vector<partida_guardada> partidas = guardado::listar();

    if (partidas.empty()) {
        QMessageBox::information(this, "Cargar partida", "Todavía no hay partidas guardadas.");
        return;
    }

    dialogo_cargar dialogo(partidas, this);
    if (dialogo.exec() == QDialog::Accepted) {
        this->accedePartida->cargar_partida(partidas[dialogo.elegida()]);
        this->accedePartida->showMaximized();
        this->hide();
    }
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

