#include "ia_super.h"
#include <vector>

ia_super::ia_super() {}

carta *ia_super::tirar_carta()
{
    carta* carta_player = this->getcarta_player();
    int tipo_player = static_cast<int>(carta_player->getTipo());
    vector<carta*> cartas_ia = this->vectorcartasvivas();

    // Buscar la carta con mayor ventaja
    carta* mejor_carta=nullptr;
    int menor = 999;
    vector<vector<int>> tabla_danio = this->getTabla_danio();
    carta* carta_ia;
    int vida_carta_player = carta_player->get_vida();

    //buscar las cartas que mas daño hagan y de esas tiene que elegir la que menos daño reciba si puede matar a la carta del jugador

    //vector <carta*> cartas_distinto_tipo;
    vector <carta*> cartas_asecinas;

    //busca cartas de distinto tipo al del player
    //for(size_t i = 0; i < cartas_ia.size(); i++)
    //{
     //   carta_ia = cartas_ia[i];
       // int tipo_ia = static_cast<int>(carta_ia->getTipo());
        //if(tipo_ia!=tipo_player){
         //   cartas_distinto_tipo.push_back(carta_ia);
       // }
    //}
    //busca las cartas que pueden matar a la del player :)
    for(int i=0; i < cartas_ia.size(); i++){
        carta_ia = cartas_ia[i];
        int tipo_ia = static_cast<int>(carta_ia->getTipo());
        if(tabla_danio[tipo_ia][tipo_player] >= vida_carta_player){
            cartas_asecinas.push_back(carta_ia);
        }
    }

    //buscar asecina que menos daño reciba
    if(cartas_asecinas.size() > 0){
        for(int i=0; i < cartas_asecinas.size(); i++){
            carta_ia = cartas_asecinas[i];
            int tipo_ia = static_cast<int>(carta_ia->getTipo());
            if (tabla_danio[tipo_player][tipo_ia] < menor) {
                menor = tabla_danio[tipo_player][tipo_ia];
                mejor_carta = carta_ia;
            }
            }
        return mejor_carta;
        }
    else{ //si ninguna mata a la carta del player entonces buscar la que mas daño haga

        cartas_ia = this->vectorcartasvivas();

        carta* peor_carta = nullptr;
        int mayor_danio_recibido = -1;

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
}