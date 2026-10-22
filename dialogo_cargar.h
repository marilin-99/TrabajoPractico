#ifndef DIALOGO_CARGAR_H
#define DIALOGO_CARGAR_H

#include <QDialog>
#include <QListWidget>
#include <vector>
#include "guardado.h"

// Mini ventana para elegir una de las partidas guardadas
class dialogo_cargar : public QDialog
{
public:
    dialogo_cargar(const vector<partida_guardada> &partidas, QWidget *parent = nullptr);

    // Posición de la partida elegida dentro del vector recibido
    int elegida() const;

private:
    QListWidget *lista;
};

#endif // DIALOGO_CARGAR_H
