#ifndef JUGADOR_H
#define JUGADOR_H
#include <vector>
#include "carta.h"

using namespace std;

class jugador
{
private:
    vector<carta> cartas;
public:
    jugador();
    void estado_cartas();
    carta getcartas(int index);
    virtual carta* tirar_carta()=0;
    vector <carta> *getvectorcartas();
    void setCartas(carta n);
};

#endif // JUGADOR_H
