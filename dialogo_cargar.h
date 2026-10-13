#ifndef DIALOGO_CARGAR_H
#define DIALOGO_CARGAR_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <vector>
#include "guardado.h"

// Ventana para elegir cuál de las partidas guardadas se quiere cargar.
class dialogo_cargar : public QDialog
{
public:
    // "danadas" es la cantidad de archivos de la carpeta que no se pudieron
    // leer (se avisa en la ventana).
    dialogo_cargar(const std::vector<partida_guardada> &partidas,
                   int danadas,
                   QWidget *parent = nullptr);

    // Posición, dentro de la lista recibida, de la partida elegida (-1 si no hay).
    int elegida() const;

private:
    QListWidget *lista;
    QPushButton *boton_cargar;
};

#endif // DIALOGO_CARGAR_H
