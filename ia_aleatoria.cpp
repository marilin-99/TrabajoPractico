#include "ia_aleatoria.h"
#include <cstdlib>
#include <ctime>

ia_aleatoria::ia_aleatoria() {}

carta *ia_aleatoria::tirar_carta()
{
    vector<carta*> cartas_ia = this->vectorcartasvivas();

    if (cartas_ia.empty()) {
        return nullptr;
    }

    return cartas_ia[rand() % cartas_ia.size()];
}
