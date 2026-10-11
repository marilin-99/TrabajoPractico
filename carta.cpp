#include "carta.h"

void carta::setVida(int newVida)
{
    vida = newVida;
}

carta::carta(TipoElemento tipo_)
{
    this->tipo = tipo_;
    this->vida = 100;
    this->estado = true;
}

TipoElemento carta::getTipo() const
{
    return tipo;
}

bool carta::getEstado() const
{
    return this->estado;
}



void carta::cambia_estado()
{
    if(this->vida <= 0)
    {
        this->estado = false;
    }
    else{
        this->estado = true;
    }
}

int carta::get_vida()
{
    return this->vida;
}

void carta::recibir_danio(int porcentaje)
{
    this->vida = this->vida-porcentaje;
    if(this->vida<0)
        this->vida=0;
}



