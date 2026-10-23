#ifndef LECTORCONFIGURACION_H
#define LECTORCONFIGURACION_H

#include <string>
#include <vector>

#include "configuracion.h"
using namespace std;

enum Codigo {
    OK, ERROR_APERTURA, ERROR_FORMATO, ARGUMENTO_INVALIDO
};

class LectorConfiguracion
{
public:
    bool leer(const string& ruta, Configuracion& config);

    void escribir_archivo(const string& ruta, const Configuracion& config);

    Codigo get_codigo() const;
    string mensaje() const;

private:
    void procesar_linea(Configuracion& config, const string& linea);
    void interpretar_danio(Configuracion& config, const string& valor);
    int buscar_tipo(const string& nombre) const;

    vector<string> nombres_tipos = {"Fuego", "Agua", "Tierra", "Viento"};
    Codigo ok = OK;
};

#endif // LECTORCONFIGURACION_H