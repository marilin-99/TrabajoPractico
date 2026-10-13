#ifndef FORMATO_PARTIDA_H
#define FORMATO_PARTIDA_H

#include <cstdint>
#include <string>
#include <vector>
#include "carta.h"

// Todo lo necesario para poder reanudar una partida más adelante.
struct datos_partida
{
    std::string nombre;                // nombre del jugador (texto UTF-8)
    std::int64_t fecha = 0;            // segundos desde 1970 (0 = que se complete al guardar)
    int dificultad_ia = 0;             // 1 aleatoria, 2 estratégica, 3 super
    std::vector<carta> cartas_player;  // cartas del jugador con su vida actual
    std::vector<carta> cartas_ia;      // cartas de la IA con su vida actual
};

// Convierte la partida en los bytes que se escriben en el archivo binario.
//
// Formato (todos los números enteros en little-endian):
//
//   4 bytes   marca "TPPG"
//   4 bytes   versión del formato (u32, hoy 1)
//   8 bytes   fecha: segundos desde 1970 (u64)
//   4 bytes   dificultad de la IA (i32)
//   4 bytes   largo del nombre en bytes (u32)  -> L
//   L bytes   nombre del jugador (UTF-8, sin terminador)
//   --- cartas del jugador ---
//   4 bytes   cantidad de cartas (u32)         -> N
//   N veces:  1 byte  tipo (0 fuego, 1 agua, 2 tierra, 3 viento)
//             4 bytes vida (i32; 0 = carta eliminada)
//   --- cartas de la IA (igual que las del jugador) ---
//   4 bytes   cantidad de cartas (u32)
//   N veces:  1 byte tipo + 4 bytes vida
std::string serializar_partida(const datos_partida &datos);

// Nombre del archivo donde se guarda la partida de ese jugador, por ejemplo
// "partida_Ana.bin". Saca los caracteres que no sirven en un nombre de
// archivo, así que nunca puede apuntar a otra carpeta.
std::string nombre_de_archivo(const std::string &nombre);

#endif // FORMATO_PARTIDA_H
