#ifndef JUEGO_H
#define JUEGO_H
#include "jugador.h"
#include <vector>
#include "carta.h"
#include "ia.h"
#include "persona.h"
#include "configuracion.h"

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
    int dificultad_actual = 0; // 1 aleatoria, 2 estratégica, 3 super (0 = sin IA)
    //carta* carta_interfaz_pl;
public:
    juego();
    void set_ia(int dificultad);
    int getDificultad() const;
    void repartir_cartas();
    // Reemplaza las cartas por las de una partida guardada (antes hay que llamar a set_ia)
    void restaurar_cartas(const vector<carta> &cartas_player, const vector<carta> &cartas_ia);
    void turnos();
    carta *getcarta_player();
    carta *getcarta_ia();
    void validar_cartas();
    vector <carta> *getcartas_ia();
    vector <carta> *getcartas_player();
    void aplicar_danio();
    int mostrar_danios_player();
    int mostrar_danios_ia();

    void carta_interfaz(carta *carta_elegida);
    vector<vector<int> > getTabla_danio() const;
    void tabla_danio_ia();
    int validar_partida();
    carta* mostrar_carta_ia();
    void aplicar_configuracion(const Configuracion& config);
    ~juego();
    void setCant_cartas(int newCant_cartas);

};

#endif // JUEGO_H