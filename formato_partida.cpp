#include "formato_partida.h"

namespace {

void escribir_u32(std::string &salida, std::uint32_t valor)
{
    for (int i = 0; i < 4; i++)
        salida.push_back(static_cast<char>((valor >> (8 * i)) & 0xFF));
}

void escribir_u64(std::string &salida, std::uint64_t valor)
{
    for (int i = 0; i < 8; i++)
        salida.push_back(static_cast<char>((valor >> (8 * i)) & 0xFF));
}

void escribir_cartas(std::string &salida, const std::vector<carta> &cartas)
{
    escribir_u32(salida, static_cast<std::uint32_t>(cartas.size()));
    for (const carta &c : cartas) {
        salida.push_back(static_cast<char>(static_cast<std::uint8_t>(c.getTipo())));
        // la vida va como entero de 32 bits con signo
        escribir_u32(salida, static_cast<std::uint32_t>(c.get_vida()));
    }
}

} // namespace

std::string serializar_partida(const datos_partida &datos)
{
    std::string salida;

    salida.append("TPPG", 4);                                        // marca
    escribir_u32(salida, 1);                                         // versión
    escribir_u64(salida, static_cast<std::uint64_t>(datos.fecha));   // fecha
    escribir_u32(salida, static_cast<std::uint32_t>(datos.dificultad_ia));

    escribir_u32(salida, static_cast<std::uint32_t>(datos.nombre.size()));
    salida.append(datos.nombre);

    escribir_cartas(salida, datos.cartas_player);
    escribir_cartas(salida, datos.cartas_ia);

    return salida;
}

std::string nombre_de_archivo(const std::string &nombre)
{
    // Los espacios, los caracteres de control y los que no se pueden usar en
    // un nombre de archivo (\ / : * ? " < > |) pasan a ser "_". Los bytes de
    // letras con tilde u otros alfabetos (>= 0x80) se dejan como están.
    const std::string prohibidos = "\\/:*?\"<>|";
    std::string limpio;
    for (unsigned char c : nombre) {
        bool invalido = c < 0x20 || c == 0x7F || c == ' ' ||
                        prohibidos.find(static_cast<char>(c)) != std::string::npos;
        if (!invalido)
            limpio.push_back(static_cast<char>(c));
        else if (limpio.empty() || limpio.back() != '_')
            limpio.push_back('_');
    }

    // Máximo 40 bytes, sin cortar una letra UTF-8 por la mitad
    if (limpio.size() > 40) {
        size_t corte = 40;
        while (corte > 0 && (static_cast<unsigned char>(limpio[corte]) & 0xC0) == 0x80)
            corte--;
        limpio.resize(corte);
    }

    // Sin "_" ni "." al principio o al final
    size_t inicio = limpio.find_first_not_of("_.");
    if (inicio == std::string::npos)
        limpio.clear();
    else
        limpio = limpio.substr(inicio, limpio.find_last_not_of("_.") - inicio + 1);

    if (limpio.empty())
        limpio = "sin_nombre";

    return "partida_" + limpio + ".bin";
}
