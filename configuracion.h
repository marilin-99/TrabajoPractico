#ifndef CONFIGURACION_H
#define CONFIGURACION_H
#include <vector>
#include <cstddef>
using namespace std;

class Configuracion
{
public:
    using Matriz = vector<vector<int>>;
    explicit Configuracion(const Matriz& tablaPorDefector, int cantidadElementos = 5);
    void cantidad_elementos(int n);
    int get_cantidad() const;
    void tipo_ia(int ia);
    int get_tipo_ia() const;
    bool tiene_tipo_ia() const;
    const Matriz& get_tabla() const;
    void set_danio(size_t fila, size_t columna, int valor);
private:
    int cant_elementos;
    int ia_elegida;
    Matriz tabla_danio;

};

#endif // CONFIGURACION_H
