#ifndef LECTORCONFIGURACION_H
#define LECTORCONFIGURACION_H

#include <functional>
#include <map>
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
    LectorConfiguracion();

    bool leer(const string &ruta, Configuracion& config);

    void escribir_archivo(const string& ruta, const Configuracion& config) const;
    string mensaje_error() const;


private:
    using ManejaArchivo = function<void(Configuracion&, const string&)>;

    void procesar_linea(Configuracion& config, const string &linea, int numero_linea);
    bool interpretar_danio(Configuracion& config, const string &valor) const;
    bool interpretar_ia(Configuracion& config, const string &valor) const;

    int buscar_tipo(const string &nombre) const;
    static int convertir_a_entero(const string& texto, bool &exito);
    static string recortar(const string &texto);
    static string minusculas(string texto);

    vector<string> nombres_tipos;

    map<string, int> indice_tipos;
    map<string, int> indice_ias;
    map<string, ManejaArchivo> manejador;
    mutable Codigo ok;
};

#endif // LECTORCONFIGURACION_H