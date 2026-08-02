#ifndef IA_ESTRATEGICA_H
#define IA_ESTRATEGICA_H
#include "jugador.h"
#include "ia.h"

class ia_estrategica: public ia
{
private:

public:
    ia_estrategica();
    carta * tirar_carta() override;
};

#endif // IA_ESTRATEGICA_H
