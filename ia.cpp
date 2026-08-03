#include "ia.h"
#include <QDebug>


carta *ia::getcarta_player() const
{
    return carta_player;
}


vector<vector<int> > ia::getTabla_danio() const
{
    return tabla_danio;
}


vector<carta*> ia::vectorcartasvivas()
{
    vector<carta*> cartas_vivas;

    vector<carta>* cartas_ia = getvectorcartas();

    for(size_t i=0;i<cartas_ia->size();i++)
    {
        if(cartas_ia->at(i).getEstado())
        {
            cartas_vivas.push_back(&cartas_ia->at(i));
        }
    }

    return cartas_vivas;
}

void ia::setTabla_danio(const vector<vector<int> > &newTabla_danio)
{
    this->tabla_danio = newTabla_danio;
}

ia::ia() {

}

void ia::setIa_dificultad(int newIa_dificultad)
{
    this->ia_dificultad = newIa_dificultad;
}

void ia::setcarta_player(carta* carta_player)
{
    this->carta_player = carta_player;
}



