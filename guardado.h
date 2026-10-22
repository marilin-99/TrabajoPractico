#ifndef GUARDADO_H
#define GUARDADO_H

#include <QString>
#include <vector>
#include "carta.h"

using namespace std;

// Los datos que se guardan de una partida
struct partida_guardada
{
    QString nombre;                 // nombre del jugador
    int dificultad = 0;             // 1 aleatoria, 2 estratégica, 3 super
    vector<carta> cartas_player;    // cartas del jugador con su vida
    vector<carta> cartas_ia;        // cartas de la IA con su vida
};

class guardado
{
public:
    // Guarda la partida en un archivo binario. Devuelve false si no pudo.
    static bool guardar(const partida_guardada &partida);

    // Devuelve todas las partidas guardadas que se pudieron leer
    static vector<partida_guardada> listar();
};

#endif // GUARDADO_H
