#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogo_cargar.h"
#include "guardado.h"
#include <QMessageBox>
#include "configuracion.h"
#include "lectorconfiguracion.h"
#include <QCoreApplication>
#include <QDebug>
#include <QStatusBar>
#include <QLineEdit>
#include <QApplication>
#include <QTimer>


using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->caja_nombre->setPlaceholderText("Ingrese nombre del jugador");
    QString nombre = ui->caja_nombre->toPlainText();
    ui->cantidad_cartas->setMinimum(0);
    ui->cantidad_cartas->setValue(0);

    QLineEdit *campo = ui->cantidad_cartas->findChild<QLineEdit *>();
    campo->setPlaceholderText("Ingrese cantidad de cartas");
    ui->cantidad_cartas->setKeyboardTracking(false);
    QTimer::singleShot(0, ui->cantidad_cartas, &QAbstractSpinBox::clear);
    connect(ui->cantidad_cartas, &QSpinBox::editingFinished, this, [this, campo]() {
        if (campo->text() == "0")
            ui->cantidad_cartas->clear();
    });

    ui->verticalSpacer_7->changeSize(20, 150, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalSpacer_5->changeSize(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalSpacer_3->changeSize(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);
    ui->verticalSpacer_2->changeSize(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);
    ui->gridLayout_2->invalidate();

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

    this->statusBar()->showMessage(QString::fromStdString(lector.mensaje()), 5000);

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
    if (!this->tabla)
        this->tabla = new tabla_danio_ventana(&this->juego, this);
    this->tabla->show();
}


void MainWindow::on_reglas_clicked()
{
    this->reglas->show();
}

