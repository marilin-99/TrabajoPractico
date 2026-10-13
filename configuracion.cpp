#include "configuracion.h"
#include <stdexcept>
using namespace std;
Configuracion::Configuracion(const Matriz &tablaPorDefector, int cantidadElementos)
{
    this->tabla_danio = tablaPorDefector;
    this->cant_elementos = cantidadElementos;
}

void Configuracion::cantidad_elementos(int n)
{
    if(n < 1){
        throw;
    }
    this->cant_elementos = n;
}

int Configuracion::get_cantidad() const
{
    return this->cant_elementos;
}

void Configuracion::tipo_ia(int ia)
{
    if((ia < 1 ) || (ia > 3)){
        throw;
    }
    this->ia_elegida = ia;
}

int Configuracion::get_tipo_ia() const
{
    return this->ia_elegida;
}

bool Configuracion::tiene_tipo_ia() const
{
    return this->ia_elegida != 0;
}

const Configuracion::Matriz &Configuracion::get_tabla() const
{
    return this->tabla_danio;
}

void Configuracion::set_danio(size_t fila, size_t columna, int valor)
{
    if(fila >= this->tabla_danio.size() || columna >= this->tabla_danio[fila].size()){
        throw;
    }
    if(valor < 0){
        throw;
    }
    this->tabla_danio[fila][columna] = valor;
}


