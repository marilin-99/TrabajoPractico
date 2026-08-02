#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized();
    this->accedePartida = new partida(this);
    ui->IniciaPartida->setEnabled(0);
    this->reglas = new class reglas(this);
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
    this->accedePartida->asignar_cartas();

    this->accedePartida->showMaximized();
    this->hide();
}



void MainWindow::on_tabla_danio_boton_clicked()
{
    this->tabla = new tabla_danio_ventana(&this->juego);
    this->tabla->show();
}


void MainWindow::on_reglas_clicked()
{
    this->reglas->show();
}

