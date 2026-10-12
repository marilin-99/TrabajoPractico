#ifndef JUEGO_H
#define JUEGO_H
#include "jugador.h"
#include <vector>
#include "carta.h"
#include "ia.h"
#include "persona.h"
#include <string>

using namespace std;

class juego
{
private:
    ia* ia;
    persona* player;
    vector <vector<int>> tabla_danio;
    carta* carta_ia;
    carta* carta_player;
    static const TipoElemento elementos[4];
    int cant_cartas;
    //carta* carta_interfaz_pl;
    int dificultad = 0;

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
    bool guardar_partida(const std::string &ruta);
    bool cargar_partida(const std::string &ruta);
};

#endif // JUEGO_H