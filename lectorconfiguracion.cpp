#include "lectorconfiguracion.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;
LectorConfiguracion::LectorConfiguracion()
{
    nombres_tipos = {"Fuego", "Agua", "Tierra", "Viento"};
    for (size_t i = 0; i < nombres_tipos.size(); ++i) {
        indice_tipos[minusculas(nombres_tipos[i])] = static_cast<int>(i);
    }

    indice_ias = {
                  {"aleatoria", 1}, {"1", 1},
                  {"estrategica", 2}, {"2", 2},
                  {"super", 3}, {"3", 3},
                  };

    manejador = {
                 {"cantidad_elementos",
                  [](Configuracion& c, const string& v) {
                      bool exito = true;
                      int val = convertir_a_entero(v, exito);
                      if (exito) c.cantidad_elementos(val);
                  }},
                 {"tipo_ia",
                  [this](Configuracion& c, const string& v) { interpretar_ia(c, v); }},
                 {"dano",
                  [this](Configuracion& c, const string& v) { interpretar_danio(c, v); }},
                 };
}

bool LectorConfiguracion::leer(const string& ruta, Configuracion& config)
{
    this->ok = OK;
    ifstream archivo(ruta);
    if (!archivo) {
        ok = ERROR_APERTURA;
        return false;
    }

    string linea;
    int numero_linea = 0;
    while (getline(archivo, linea)) {
        ++numero_linea;
        procesar_linea(config, linea, numero_linea);
        if(this->ok != OK){
            return false;
        }
    }
    return true;
}

void LectorConfiguracion::escribir_archivo(const string& ruta, const Configuracion& config) const
{
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        this->ok = ERROR_APERTURA;
        return;
    }
    this->ok = OK;
    archivo << "Configuracion del juego\n";
    archivo << "Formato: clave=valor\n";

    archivo << "Cantidad de elementos por jugador\n";
    archivo << "cantidad_elementos=" << config.get_cantidad() << "\n\n";

    archivo << "Tipo de IA: 1 = aleatoria, 2 = estrategica, 3 = super\n";
    archivo << "Si no se define, el jugador la elige al iniciar la partida.\n";
    if (config.tiene_tipo_ia()) {
        archivo << "tipo_ia=" << config.get_tipo_ia() << "\n\n";
    } else {
        archivo << "tipo_ia=0\n";
    }

    archivo << "Tabla de daño: daño=Atacante,Defensor,Valor\n";
    const auto& tabla = config.get_tabla();
    for (size_t i = 0; i < tabla.size() && i < nombres_tipos.size(); ++i) {
        for (size_t j = 0; j < tabla[i].size() && j < nombres_tipos.size(); ++j) {
            archivo << "daño=" << nombres_tipos[i] << "," << nombres_tipos[j] << ","
                    << tabla[i][j] << "\n";
        }
    }
}

void LectorConfiguracion::procesar_linea(Configuracion& config, const string& linea, int numero_linea)
{
    const string texto = recortar(linea);
    if (texto.empty() || texto.front() == '#') {
        return;
    }

    const string prefijo = "Linea " + to_string(numero_linea) + ": ";

    const auto pos_igual = texto.find('=');
    if (pos_igual == string::npos) {
        this->ok = ERROR_FORMATO;
        return;
    }

    const string clave = minusculas(recortar(texto.substr(0, pos_igual)));
    const string valor = recortar(texto.substr(pos_igual + 1));

    const auto it = manejador.find(clave);
    if (it == manejador.end()) {
        this->ok = ERROR_FORMATO;
        return;
    }

    it->second(config, valor);
}

// daño=Atacante,Defensor,Valor
bool LectorConfiguracion::interpretar_danio(Configuracion& config, const string& valor) const
{
    vector<string> partes;
    stringstream ss(valor);
    string parte;
    while (getline(ss, parte, ',')) {
        partes.push_back(recortar(parte));
    }
    if (partes.size() != 3) {
        this->ok = ERROR_FORMATO;
        return false;
    }
    int at = buscar_tipo(partes[0]);
    int def = buscar_tipo(partes[1]);
    if (this->ok != OK) return false;

    bool exito = true;
    int dano = convertir_a_entero(partes[2], exito);
    if (!exito) {
        this->ok = ARGUMENTO_INVALIDO;
        return false;
    }

    config.set_danio(static_cast<size_t>(at), static_cast<size_t>(def), dano);
    return true;
}

bool LectorConfiguracion::interpretar_ia(Configuracion& config, const string& valor) const
{
    const auto it = indice_ias.find(minusculas(valor));
    if (it == indice_ias.end()) {
        this->ok = ARGUMENTO_INVALIDO;
        return false;
    }
    config.tipo_ia(it->second);
}

int LectorConfiguracion::buscar_tipo(const string& nombre) const
{
    const auto it = indice_tipos.find(minusculas(nombre));
    if (it == indice_tipos.end()) {
        this->ok = ARGUMENTO_INVALIDO;
        return -1;
    }
    return it->second;
}

int LectorConfiguracion::convertir_a_entero(const string& texto, bool& exito)
{
    size_t usados = 0;
    int numero = 0;
    try {
        numero = stoi(texto, &usados);
    } catch (const exception&) {
        exito = false;
    }
    if (usados != texto.size()) {
        exito = false;
    }
    exito = true;
    return numero;
}

string LectorConfiguracion::recortar(const string& texto)
{
    const auto inicio = texto.find_first_not_of(" \t\r\n");
    if (inicio == string::npos) {
        return "";
    }
    const auto fin = texto.find_last_not_of(" \t\r\n");
    return texto.substr(inicio, fin - inicio + 1);
}

string LectorConfiguracion::minusculas(string texto)
{
    transform(texto.begin(), texto.end(), texto.begin(), [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return texto;
}

string LectorConfiguracion::mensaje_error() const
{
    switch (this->ok) {
    case OK:
        return "fichero procesado correctamente. \n";
        break;
    case ERROR_APERTURA:
        return "Error de apertura del archivo. \n";
        break;
    case ERROR_FORMATO:
        return "Error de formato en la lectura del archivo. \n";
        break;
    case ARGUMENTO_INVALIDO:
        return "El argumento ingresado no es valido. \n";
    }
    return "Estado desconocido. \n";
}
