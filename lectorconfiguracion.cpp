#include "lectorconfiguracion.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

bool LectorConfiguracion::leer(const string& ruta, Configuracion& config)
{
    ok = OK;

    ifstream archivo(ruta);
    if (!archivo) {
        ok = ERROR_APERTURA;
        return false;
    }

    string linea;
    while (getline(archivo, linea)) {
        procesar_linea(config, linea);
    }
    return true;
}

void LectorConfiguracion::escribir_archivo(const string& ruta, const Configuracion& config)
{
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        ok = ERROR_APERTURA;
        return;
    }

    archivo << "# Configuracion del juego\n";
    archivo << "# Formato: clave=valor (las lineas que empiezan con # son comentarios)\n\n";

    archivo << "# Cantidad de elementos por jugador\n";
    archivo << "cantidad_elementos=" << config.get_cantidad() << "\n\n";

    archivo << "# Tipo de IA: 1 = aleatoria, 2 = estrategica, 3 = super\n";
    archivo << "# Si no se define, el jugador la elige al iniciar la partida.\n";
    if (config.tiene_tipo_ia()) {
        archivo << "tipo_ia=" << config.get_tipo_ia() << "\n\n";
    } else {
        archivo << "tipo_ia=2\n\n";
    }

    archivo << "# Tabla de daño: daño=Atacante,Defensor,Valor\n";
    const auto& tabla = config.get_tabla();
    for (size_t i = 0; i < tabla.size(); ++i) {
        for (size_t j = 0; j < tabla[i].size(); ++j) {
            archivo << "daño=" << nombres_tipos[i] << "," << nombres_tipos[j] << ","
                    << tabla[i][j] << "\n";
        }
    }

    ok = OK;
}

string LectorConfiguracion::mensaje() const
{
    // Mismo orden que el enum Codigo
    const string textos[] = {
        "Fichero procesado correctamente.",
        "Error de apertura del archivo.",
        "Error de formato en la lectura del archivo.",
        "El argumento ingresado no es valido."
    };
    return textos[ok];
}

void LectorConfiguracion::procesar_linea(Configuracion& config, const string& linea)
{
    if (linea.find_first_not_of(" \t") == string::npos || linea[0] == '#') {
        return;
    }

    const size_t pos_igual = linea.find('=');
    if (pos_igual == string::npos) {
        ok = ERROR_FORMATO;
        return;
    }

    const string clave = linea.substr(0, pos_igual);
    const string valor = linea.substr(pos_igual + 1);

    try {
        if (clave == "cantidad_elementos") {
            config.cantidad_elementos(stoi(valor));
        } else if (clave == "tipo_ia") {
            config.tipo_ia(stoi(valor));
        } else if (clave == "daño") {
            interpretar_danio(config, valor);
        } else {
            ok = ERROR_FORMATO;
        }
    } catch (const exception&) {
        ok = ARGUMENTO_INVALIDO;
    }
}

// dano=Atacante,Defensor,Valor   (ej: daño=Agua,Fuego,50)
void LectorConfiguracion::interpretar_danio(Configuracion& config, const string& valor)
{
    stringstream val(valor);
    string atacante, defensor, danio;

    if (!getline(val, atacante, ',') || !getline(val, defensor, ',') || !getline(val, danio)) {
        ok = ERROR_FORMATO;
        return;
    }

    config.set_danio(buscar_tipo(atacante), buscar_tipo(defensor), stoi(danio));
}

int LectorConfiguracion::buscar_tipo(const string& nombre) const
{
    for (size_t i = 0; i < nombres_tipos.size(); ++i) {
        if (nombres_tipos[i] == nombre) {
            return static_cast<int>(i);
        }
    }
    throw invalid_argument("tipo de elemento desconocido");
}