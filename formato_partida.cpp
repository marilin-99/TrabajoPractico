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

const int MAX_CARTAS = 50;   // el mismo tope que usa partida::setCant_cartas
const int BYTES_POR_CARTA = 5;
const char *const INCOMPLETO = "El archivo está incompleto o dañado.";

// Va leyendo los bytes de a poco y avisa (false) si se quiere leer más de lo que hay.
class lector
{
public:
    explicit lector(const std::string &bytes) : bytes_(bytes) {}

    size_t restante() const { return bytes_.size() - pos_; }

    bool u8(std::uint8_t &valor)
    {
        if (restante() < 1)
            return false;
        valor = static_cast<std::uint8_t>(bytes_[pos_++]);
        return true;
    }

    bool u32(std::uint32_t &valor)
    {
        if (restante() < 4)
            return false;
        valor = 0;
        for (int i = 0; i < 4; i++)
            valor |= static_cast<std::uint32_t>(static_cast<std::uint8_t>(bytes_[pos_++])) << (8 * i);
        return true;
    }

    bool u64(std::uint64_t &valor)
    {
        if (restante() < 8)
            return false;
        valor = 0;
        for (int i = 0; i < 8; i++)
            valor |= static_cast<std::uint64_t>(static_cast<std::uint8_t>(bytes_[pos_++])) << (8 * i);
        return true;
    }

    bool texto(size_t largo, std::string &valor)
    {
        if (restante() < largo)
            return false;
        valor.assign(bytes_, pos_, largo);
        pos_ += largo;
        return true;
    }

private:
    const std::string &bytes_;
    size_t pos_ = 0;
};

bool leer_cartas(lector &in, std::vector<carta> &cartas, std::string &error)
{
    std::uint32_t cantidad = 0;
    if (!in.u32(cantidad)) {
        error = INCOMPLETO;
        return false;
    }
    if (cantidad < 1 || cantidad > static_cast<std::uint32_t>(MAX_CARTAS)) {
        error = "La cantidad de cartas no es válida.";
        return false;
    }
    // Antes de leer se comprueba que los bytes alcanzan para todas las cartas
    if (in.restante() / BYTES_POR_CARTA < cantidad) {
        error = INCOMPLETO;
        return false;
    }

    for (std::uint32_t i = 0; i < cantidad; i++) {
        std::uint8_t tipo = 0;
        std::uint32_t vida_bits = 0;
        if (!in.u8(tipo) || !in.u32(vida_bits)) {
            error = INCOMPLETO;
            return false;
        }
        const int vida = static_cast<std::int32_t>(vida_bits);   // entero con signo

        if (tipo > 3) {
            error = "Hay una carta con un tipo de elemento inválido.";
            return false;
        }
        if (vida < 0 || vida > 100) {
            error = "Hay una carta con una vida inválida.";
            return false;
        }

        carta c(static_cast<TipoElemento>(tipo));
        c.setVida(vida);
        c.cambia_estado();     // vida 0 = carta eliminada
        cartas.push_back(c);
    }
    return true;
}

bool alguna_viva(const std::vector<carta> &cartas)
{
    for (const carta &c : cartas)
        if (c.getEstado())
            return true;
    return false;
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

bool leer_partida(const std::string &bytes, datos_partida &datos, std::string &error)
{
    datos = datos_partida();
    error.clear();

    datos_partida leida;
    lector in(bytes);

    std::string marca;
    if (!in.texto(4, marca) || marca != "TPPG") {
        error = "No es un archivo de partida guardada.";
        return false;
    }

    std::uint32_t version = 0;
    if (!in.u32(version)) {
        error = INCOMPLETO;
        return false;
    }
    if (version != 1) {
        error = "El archivo es de una versión que este juego no entiende (versión " +
                std::to_string(version) + ").";
        return false;
    }

    std::uint64_t fecha = 0;
    std::uint32_t dificultad_bits = 0;
    std::uint32_t largo_nombre = 0;
    if (!in.u64(fecha) || !in.u32(dificultad_bits) || !in.u32(largo_nombre)) {
        error = INCOMPLETO;
        return false;
    }

    const int dificultad = static_cast<std::int32_t>(dificultad_bits);
    if (dificultad < 1 || dificultad > 3) {
        error = "La dificultad de la IA no es válida.";
        return false;
    }

    if (!in.texto(largo_nombre, leida.nombre)) {
        error = INCOMPLETO;
        return false;
    }

    if (!leer_cartas(in, leida.cartas_player, error) ||
        !leer_cartas(in, leida.cartas_ia, error))
        return false;

    if (leida.cartas_player.size() != leida.cartas_ia.size()) {
        error = "El jugador y la IA tienen distinta cantidad de cartas.";
        return false;
    }
    if (in.restante() != 0) {
        error = "El archivo tiene datos de más.";
        return false;
    }
    if (!alguna_viva(leida.cartas_player) || !alguna_viva(leida.cartas_ia)) {
        error = "Esa partida ya había terminado.";
        return false;
    }

    leida.fecha = static_cast<std::int64_t>(fecha);
    leida.dificultad_ia = dificultad;
    datos = leida;
    return true;
}
