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
    int menor = 0;
    int mayor = 0;
    vector<vector<int>> tabla_danio = this->getTabla_danio();
    carta* carta_ia;
    int vida_carta_player = carta_player->get_vida();

    //buscar las cartas que mas daño hagan y de esas tiene que elegir la que menos daño reciba si puede matar a la carta del jugador

    vector <carta*> cartas_distinto_tipo;
    vector <carta*> cartas_asecinas;

    for(size_t i = 0; i < cartas_ia.size(); i++)
    {
        carta_ia = cartas_ia[i];
        int tipo_ia = static_cast<int>(carta_ia->getTipo());
        if(tabla_danio[tipo_ia][tipo_player]!=tipo_player){
            cartas_distinto_tipo.push_back(carta_ia);
        }
    }
    for(int i=0; i < cartas_distinto_tipo.size(); i++){
        carta_ia = cartas_distinto_tipo[i];
        int tipo_ia = static_cast<int>(carta_ia->getTipo());
        if(tabla_danio[tipo_ia][tipo_player] > vida_carta_player){
            cartas_asecinas.push_back(carta_ia);
        }
    }
    //buscar asecina que menos daño reciba
    if(cartas_asecinas.size() > 0){
        for(int i=0; i < cartas_asecinas.size(); i++){
            carta_ia = cartas_asecinas[i];
            int tipo_ia = static_cast<int>(carta_ia->getTipo());
            if(menor < tabla_danio[tipo_player][tipo_ia]){
                menor = tabla_danio[tipo_player][tipo_ia];
                mejor_carta = carta_ia;
            }
            }
        return mejor_carta;
        }
    else{ //si ninguna mata a la carta del player entonces buscar la que mas daño haga
        for(int i=0; i < cartas_distinto_tipo.size(); i++){
            carta_ia = cartas_distinto_tipo[i];
            int tipo_ia = static_cast<int>(carta_ia->getTipo());
            if(mayor < tabla_danio[tipo_ia][tipo_player]){
                mayor = tabla_danio[tipo_ia][tipo_player];
                mejor_carta = carta_ia;
            }
        }
        return mejor_carta;
    }
}
