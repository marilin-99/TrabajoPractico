#ifndef PARTIDA_H
#define PARTIDA_H
#include "juego.h"
#include "carta.h"
#include <QMainWindow>
#include <QSoundEffect>
#include "reglas.h"
#include "tabla_danio_ventana.h"



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


    void seleccion_carta();
    void setDificultad_ia(int newDificultad_ia);

    void jugar_carta(int index);
    void habilitar_botones();
    void deshabilitar_botones();
    void mostrar_carta_ia();
    void habilitar_vivas();
    void finalizar_partida();

private slots:
    void on_carta_1_clicked();

    void on_carta_2_clicked();

    void on_carta_3_clicked();

    void on_carta_4_clicked();

    void on_carta_5_clicked();

    void on_reglas_clicked();

    void on_tabla_danio_clicked();

private:
    Ui::partida *ui;
    juego juego;
    int dificultad_ia;
    QSoundEffect sonidoCarta;
    tabla_danio_ventana *tabla;
    reglas *reglas;
};

#endif // PARTIDA_H
