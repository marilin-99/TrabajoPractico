#include "jugador.h"
jugador::jugador() {}

carta jugador::getcartas(int index)
{
    return this->cartas[index];
}

vector<carta> *jugador::getvectorcartas()
{
    return &this->cartas;
}

void jugador::setCartas(carta n)
{
    this->cartas.push_back(n);
}




