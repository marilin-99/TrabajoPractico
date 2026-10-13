#ifndef GUARDADO_H
#define GUARDADO_H

#include <QString>
#include "formato_partida.h"

// Guarda las partidas en archivos binarios (el formato está explicado en
// formato_partida.h). Acá solo se decide dónde va el archivo y se escribe.
class guardado
{
public:
    // Carpeta donde se guardan las partidas.
    static QString carpeta();

    // Escribe la partida en <carpeta>/partida_<nombre>.bin (si ya existía una
    // del mismo jugador, la reemplaza). Devuelve true si salió bien; si no,
    // deja el motivo en *error. En *ruta queda la ruta del archivo escrito.
    static bool guardar(const datos_partida &datos,
                        QString *ruta = nullptr,
                        QString *error = nullptr);
};

#endif // GUARDADO_H
