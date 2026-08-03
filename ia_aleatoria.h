#ifndef IA_ALEATORIA_H
#define IA_ALEATORIA_H
#include "ia.h"

class ia_aleatoria: public ia
{
public:
    ia_aleatoria();
    carta *tirar_carta() override;
};

#endif // IA_ALEATORIA_H
