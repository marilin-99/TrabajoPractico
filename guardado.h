#ifndef GUARDADO_H
#define GUARDADO_H

#include <QString>
#include <vector>
#include "formato_partida.h"

// Una partida leída de la carpeta de partidas.
struct partida_guardada
{
    QString ruta;          // archivo del que se leyó
    datos_partida datos;   // su contenido, ya validado
};

// Guarda y carga las partidas en archivos binarios (el formato está explicado
// en formato_partida.h). Acá solo se decide dónde van los archivos y se leen
// o escriben.
class guardado
{
public:
    // Carpeta "partidas", dentro de la carpeta donde está el juego (junto al ejecutable).
    static QString carpeta();

    // Escribe la partida en <carpeta>/partida_<nombre>.bin (si ya existía una
    // del mismo jugador, la reemplaza). Devuelve true si salió bien; si no,
    // deja el motivo en *error. En *ruta queda la ruta del archivo escrito.
    static bool guardar(const datos_partida &datos,
                        QString *ruta = nullptr,
                        QString *error = nullptr);

    // Lee un archivo de partida y comprueba que sea válido. Si no lo es,
    // devuelve false y deja el motivo en *error.
    static bool cargar(const QString &ruta,
                       datos_partida &datos,
                       QString *error = nullptr);

    // Todas las partidas válidas de la carpeta, la más reciente primero.
    // En *danadas queda cuántos archivos no se pudieron leer.
    static std::vector<partida_guardada> listar(int *danadas = nullptr);
};

#endif // GUARDADO_H
