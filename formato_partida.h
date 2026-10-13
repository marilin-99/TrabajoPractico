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

// Interpreta los bytes de un archivo de partida (el formato de arriba).
// Antes de aceptarlo revisa que el archivo sea válido: la marca y la versión,
// que no esté incompleto ni tenga datos de más, y que los valores tengan
// sentido (dificultad de 1 a 3, entre 1 y 50 cartas por lado y la misma
// cantidad en ambos, tipo de 0 a 3, vida de 0 a 100 y al menos una carta
// viva de cada lado, o sea una partida que todavía se puede jugar).
// Si todo está bien devuelve true y completa "datos". Si no, devuelve false,
// deja "datos" vacío y escribe el motivo en "error".
bool leer_partida(const std::string &bytes, datos_partida &datos, std::string &error);

// Nombre del archivo donde se guarda la partida de ese jugador, por ejemplo
// "partida_Ana.bin". Saca los caracteres que no sirven en un nombre de
// archivo, así que nunca puede apuntar a otra carpeta.
std::string nombre_de_archivo(const std::string &nombre);

#endif // FORMATO_PARTIDA_H
