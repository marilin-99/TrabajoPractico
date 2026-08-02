#ifndef IA_H
#define IA_H
#include "jugador.h"
#include "carta.h"
#include <vector>

class ia : public jugador
{
private:
    int ia_dificultad;
    carta *carta_player=nullptr;
    vector <carta> *cartas_ia;
    vector <vector<int>> tabla_danio;
public:
    ia();
    virtual carta *tirar_carta()=0;
    void setIa_dificultad(int newIa_dificultad);
    void setcarta_player(carta* carta_player);

    carta *getcarta_player() const;

    vector<vector<int> > getTabla_danio() const;
    vector <carta*> vectorcartasvivas();
    void setTabla_danio(const vector<vector<int> > &newTabla_danio);
};

#endif // IA_H
