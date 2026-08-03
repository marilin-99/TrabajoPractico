#ifndef IA_SUPER_H
#define IA_SUPER_H
#include "ia.h"

class ia_super : public ia
{
public:
    ia_super();
    carta *tirar_carta() override;
};

#endif // IA_SUPER_H
