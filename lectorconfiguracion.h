#ifndef LECTORCONFIGURACION_H
#define LECTORCONFIGURACION_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "configuracion.h"
using namespace std;
class LectorConfiguracion
{
public:
    LectorConfiguracion();

    // Devuelve false si el archivo no existe (configuracion queda intacta).
    bool leer(const string &ruta, Configuracion& config);

    // Escribe un archivo de ejemplo con los valores actuales de configuracion.
    void escribir_archivo(const string& ruta, const Configuracion& config) const;

    // Problemas encontrados en la ultima lectura (para informar al usuario).
    const vector<string> &errores() const;

private:
    using ManejaArchivo = function<void(Configuracion&, const string&)>;

    void procesar_linea(Configuracion& config, const string &linea, int numero_linea);
    void interpretar_danio(Configuracion& config, const string &valor) const;
    void interpretar_ia(Configuracion& config, const string &valor) const;

    int buscar_tipo(const string &nombre) const;
    static int convertir_a_entero(const string& texto);
    static string recortar(const string &texto);
    static string minusculas(string texto);

    // Orden de filas/columnas de la tabla (el mismo que los encabezados de la ventana)
    vector<string> nombres_tipos;
    // Tablas de busqueda: reemplazan cadenas de if/else
    map<string, int> indice_tipos;
    map<string, int> indice_ias;
    map<string, ManejaArchivo> manejador;
    vector<string> lista_errores;
};

#endif // LECTORCONFIGURACION_H