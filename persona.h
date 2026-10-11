#ifndef PERSONA_H
#define PERSONA_H
#include "carta.h"
#include "jugador.h"

class persona : public jugador
{
private:
    carta *carta_elegida=nullptr;
public:
    persona();
    carta * tirar_carta() override;
    void setCarta_elegida(carta *newCarta_elegida);
    carta *getCarta_elegida() const;
};

#endif // PERSONA_H
