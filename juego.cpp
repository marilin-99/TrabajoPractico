#include "juego.h"
#include "ia_aleatoria.h"
#include "ia_estrategica.h"
#include "ia_super.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <QDebug>

void juego::setCant_cartas(int newCant_cartas)
{
    cant_cartas = newCant_cartas;
}

juego::juego() {
    tabla_danio = vector<vector<int>>(4, vector<int>(4));
    tabla_danio =
        {
            //              Fuego Agua Tierra Viento
            /* Fuego */ {   10,    20,   50,    35 },
            /* Agua  */ {   50,    10,   35,    20 },
            /* Tierra*/ {   20,    35,   10,    50 },
            /* Viento*/ {   35,    50,   20,    10 }
        };
    player = new persona();
    ia = nullptr;
}

void juego::set_ia(int dificultad)
{
    delete ia;
    ia = nullptr;

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

    // Revisar todas las cartas del jugador.
    for (int i = 0; i < cartas_player->size(); i++)
    {
        if (cartas_player->at(i).getEstado())
        {
            player_perdio = false;
            break;
        }
    }

    // Revisar todas las cartas de la IA.
    for (int i = 0; i < cartas_ia->size(); i++)
    {
        if (cartas_ia->at(i).getEstado())
        {
            ia_perdio = false;
            break;
        }
    }

    if (player_perdio)
        return 1; // Gana la IA.

    if (ia_perdio)
        return 2; // Gana el jugador.

    return 0; // La partida continúa.
}

carta* juego::mostrar_carta_ia()
{
    return this->carta_ia;
}


juego::~juego()
{
    delete ia;
    delete player;
}


carta* juego::getcarta_player()
{
   return this->player->tirar_carta();
}

carta* juego::getcarta_ia()
{
    return this->ia->tirar_carta();
}


const TipoElemento juego::elementos[4] = {TipoElemento::FUEGO, TipoElemento::AGUA, TipoElemento::TIERRA, TipoElemento::VIENTO};
void juego::repartir_cartas()
{
    // Eliminar las cartas de la partida anterior
    this->player->getvectorcartas()->clear();
    this->ia->getvectorcartas()->clear();

    // Repartir cinco cartas nuevas a la IA
    for (int i = 0; i < cant_cartas; i++) {
        TipoElemento tipo_random_ia = elementos[rand() % 4];
        this->ia->setCartas(carta(tipo_random_ia));
    }

    // Repartir cinco cartas nuevas al jugador
    for (int i = 0; i < cant_cartas; i++) {
        TipoElemento tipo_random_player = elementos[rand() % 4];
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




