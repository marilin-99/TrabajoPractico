#include "persona.h"

persona::persona() {
    carta_elegida = nullptr;
}

void persona::setCarta_elegida(carta *newCarta_elegida)
{
    this->carta_elegida = newCarta_elegida;
}

carta *persona::getCarta_elegida() const
{
    return carta_elegida;
}


carta *persona::tirar_carta()
{
    return this->carta_elegida;
}
