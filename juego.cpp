#include "juego.h"
#include "ia_aleatoria.h"
#include "ia_estrategica.h"
#include "ia_super.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <QDebug>

juego::juego() {
    tabla_danio = tabla_danio = vector<vector<int>>(3, vector<int>(3));
    tabla_danio =
        {
            {10, 20, 50},   // Fuego
            {50, 10, 20},   // Agua
            {20, 50, 10}    // Tierra
        };
    player = new persona();
    ia = nullptr;
}

void juego::set_ia(int dificultad)
{
        switch(dificultad)
        {
        case 1:
            ia = new ia_aleatoria();
            break;

        case 2:
            ia = new ia_estrategica();
            break;

        case 3:
            ia = new ia_super();
            break;
    }

}

vector<vector<int> > juego::getTabla_danio() const
{
    return tabla_danio;
}

void juego::tabla_danio_ia()
{
    this->ia->setTabla_danio(this->tabla_danio);
}

int juego::validar_partida()
{
    vector<carta> *cartas_player = this->getcartas_player();
    vector<carta> *cartas_ia = this->getcartas_ia();

    bool player_perdio = true;
    bool ia_perdio = true;

    for (int i = 0; i < 5; i++)
    {
        if (cartas_player->at(i).getEstado())
            player_perdio = false;

        if (cartas_ia->at(i).getEstado())
            ia_perdio = false;
    }

    if (player_perdio){
        return 1;   // Gana la IA
    }
    if (ia_perdio){
        return 2;   // Gana el jugador
    }
    return 0;       // La partida continúa
}

carta* juego::mostrar_carta_ia()
{
    return this->carta_ia;
}


carta* juego::getcarta_player()
{
   return this->player->tirar_carta();
}

carta* juego::getcarta_ia()
{
    return this->ia->tirar_carta();
}


const TipoElemento juego::elementos[3] = {TipoElemento::FUEGO, TipoElemento::AGUA, TipoElemento::TIERRA};
void juego::repartir_cartas()
{
    TipoElemento tipo_random_ia;
    for(int i=0; i<5; i++){
        tipo_random_ia = elementos[rand() % 3];
        this->ia->setCartas(carta(tipo_random_ia));
    }


    TipoElemento tipo_random_player;
    for(int i=0; i<5; i++){
        tipo_random_player = elementos[rand() % 3];
        this->player->setCartas(carta(tipo_random_player));
    }

}

void juego::validar_cartas()
{
    this->carta_player->cambia_estado();
    this->carta_ia->cambia_estado();
}

vector<carta> *juego::getcartas_ia()
{
    vector <carta> *cartas_ia = this->ia->getvectorcartas();
    return cartas_ia;
}

vector<carta> *juego::getcartas_player()
{
    return this->player->getvectorcartas();
}

void juego::aplicar_danio()
{
    int tipo_player = static_cast<int>(carta_player->getTipo() );
    int tipo_ia = static_cast<int>(carta_ia->getTipo() );
    this->carta_player->recibir_danio(tabla_danio[tipo_ia][tipo_player]);
    this->carta_ia->recibir_danio(tabla_danio[tipo_player][tipo_ia]);

}

vector<carta> *juego::vector_cartas_player()
{
    return this->player->getvectorcartas();
}

void juego::carta_interfaz(carta *carta_elegida)
{
    this->player->setCarta_elegida(carta_elegida);
}

void juego::turnos()
{
    this->carta_player = this->getcarta_player(); //ataque de player
    this->ia->setcarta_player(this->carta_player); //pasa la carta usada por player a ia
    this->carta_ia = this->getcarta_ia(); //ataque de ia
    aplicar_danio();
    validar_cartas();
}




