#ifndef JUEGO_H
#define JUEGO_H
#include "jugador.h"
#include <vector>
#include "carta.h"
#include "ia.h"
#include "persona.h"

using namespace std;

class juego
{
private:
    ia* ia = nullptr;
    persona* player = nullptr;
    vector <vector<int>> tabla_danio;
    carta* carta_ia = nullptr;
    carta* carta_player = nullptr;
    static const TipoElemento elementos[4];
    int cant_cartas = 5;
    //carta* carta_interfaz_pl;

public:
    juego();
    void set_ia(int dificultad);
    void repartir_cartas();
    void turnos();
    carta *getcarta_player();
    carta *getcarta_ia();
    void validar_cartas();
    vector <carta> *getcartas_ia();
    vector <carta> *getcartas_player();
    void aplicar_danio();

    void carta_interfaz(carta *carta_elegida);
    vector<vector<int> > getTabla_danio() const;
    void tabla_danio_ia();
    int validar_partida();
    carta* mostrar_carta_ia();
    ~juego();
    void setCant_cartas(int newCant_cartas);
};

#endif // JUEGO_H