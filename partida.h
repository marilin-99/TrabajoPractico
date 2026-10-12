#ifndef PARTIDA_H
#define PARTIDA_H
#include "juego.h"
#include <QMainWindow>
#include <QSoundEffect>
#include "qprogressbar.h"
#include "reglas.h"
#include "tabla_danio_ventana.h"
#include <QPushButton>
#include <vector>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QGridLayout>
#include <QVBoxLayout>


namespace Ui {
class partida;
}

class partida : public QMainWindow
{
    Q_OBJECT

public:
    explicit partida(QWidget *parent = nullptr);
    ~partida();
    void asignar_cartas();
    void actualizar_vida();

    void setDificultad_ia(int newDificultad_ia);

    void jugar_carta(int index);
    void deshabilitar_botones();
    void mostrar_carta_ia();
    void habilitar_vivas();
    void finalizar_partida();

    void setCant_cartas(int newCant_cartas);

    void crearBotonesCartasIA();
    void actualizar_vida_ia();
    void actualizar_cartas_ia();


    void crearCartasIA();
    void actualizarVidaIA();

private slots:
    void on_reglas_clicked();

    void on_tabla_danio_clicked();

private:
    Ui::partida *ui;
    QGridLayout *layoutCartas;

    QVector<QPushButton*> botonesCartas;
    QVector<QProgressBar*> barrasVida;

    QGridLayout *layoutCartasIA;
   vector<QLabel*> imagenesCartasIA;
    vector<QProgressBar*> barrasVidaIA;


    juego juego;
    int cant_cartas=5;
    int dificultad_ia;

    QSoundEffect sonidoCarta;
    tabla_danio_ventana *tabla;
    reglas *reglas;

    void crearBotonesCartas();
};

#endif // PARTIDA_H
