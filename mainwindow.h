#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ia.h>
#include <partida.h>
#include <ia_aleatoria.h>
#include <ia_estrategica.h>
#include <ia_super.h>
#include "tabla_danio_ventana.h"
#include "juego.h"
#include "reglas.h"
#include <Qstring>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_elegir_IA_currentIndexChanged(int index);
    void on_IniciaPartida_clicked();
    void iniciar_partida();

    void on_tabla_danio_boton_clicked();

    void on_reglas_clicked();

private:
    Ui::MainWindow *ui;
    partida *accedePartida;
    ia *IA_elegida;
    tabla_danio_ventana *tabla = nullptr;
    juego juego;
    int index = 0;
    reglas *reglas;
    QString nombre;
};

#endif // MAINWINDOW_H
