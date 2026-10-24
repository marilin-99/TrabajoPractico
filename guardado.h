#ifndef GUARDADO_H
#define GUARDADO_H

#include <QString>
#include <vector>
#include "carta.h"

using namespace std;

class QDataStream; // declaración adelantada, el include va en el .cpp

// Los datos que se guardan de una partida
struct partida_guardada
{
    QString nombre;                // nombre del jugador
    int dificultad = 0;            // 1 aleatoria, 2 estratégica, 3 super
    vector<carta> cartas_player;   // cartas del jugador con su vida
    vector<carta> cartas_ia;       // cartas de la IA con su vida
};

class guardado
{
public:
    // Guarda la partida en un archivo binario. Devuelve false si no pudo.
    static bool guardar(const partida_guardada &partida);

    // Devuelve todas las partidas guardadas que se pudieron leer
    static vector<partida_guardada> listar();

private:
    // Carpeta "partidas", dentro de la carpeta donde está el juego
    static QString carpeta();

    // Arma la ruta partida_<nombre>.bin (limpia caracteres inválidos del nombre)
    static QString ruta_archivo(const QString &nombre);

    // De cada carta se guarda el tipo y la vida
    static void escribir_cartas(QDataStream &out, const vector<carta> &cartas);

    // Lee 'cantidad' cartas. Devuelve false si los datos están mal
    // o si no queda ninguna carta con vida
    static bool leer_cartas(QDataStream &in, int cantidad, vector<carta> &cartas);

    // Lee un archivo de partida. Devuelve false si está dañado o modificado a mano
    static bool leer_partida(const QString &ruta, partida_guardada &partida);
};

#endif // GUARDADO_H