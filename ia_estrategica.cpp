#include "ia_estrategica.h"
#include <vector>
#include <QDebug>

ia_estrategica::ia_estrategica() {}

carta *ia_estrategica::tirar_carta()
{
    carta* carta_player = this->getcarta_player();
    int tipo_player = static_cast<int>(carta_player->getTipo());

    vector<carta*> cartas_ia = this->vectorcartasvivas();

    carta* peor_carta = nullptr;
    int mayor_danio_recibido = -1;

    vector<vector<int>> tabla_danio = this->getTabla_danio();

    for(size_t i = 0; i < cartas_ia.size(); i++)
    {
        carta* carta_ia = cartas_ia[i];
        int tipo_ia = static_cast<int>(carta_ia->getTipo());

        // Daño que el jugador le haría a esta carta
        int danio_recibido = tabla_danio[tipo_ia][tipo_player];

        if(danio_recibido > mayor_danio_recibido)
        {
            mayor_danio_recibido = danio_recibido;
            peor_carta = carta_ia;
        }
    }

    return peor_carta;
}