#include "ia_aleatoria.h"
#include "qdebug.h"
#include "qlogging.h"
#include <cstdlib>
#include <ctime>

ia_aleatoria::ia_aleatoria() {}

carta *ia_aleatoria::tirar_carta()
{
    static bool inicializado = false;

    if(!inicializado)
    {
        srand(time(NULL));
        inicializado = true;
    }

    vector<carta*> cartas_ia = this->vectorcartasvivas();

    return cartas_ia[rand() % cartas_ia.size()];
}
