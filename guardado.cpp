#include "guardado.h"
#include <QCoreApplication>
#include <QDataStream>
#include <QDir>
#include <QFile>

// Carpeta "partidas", dentro de la carpeta donde está el juego
static QString carpeta()
{
    return QCoreApplication::applicationDirPath() + "/partidas";
}

// El archivo se llama partida_<nombre>.bin.
// Lo que no es letra ni número se cambia por "_" para que sea un nombre de archivo válido.
static QString ruta_archivo(const QString &nombre)
{
    QString limpio;
    for (QChar c : nombre.left(40)) {
        if (c.isLetterOrNumber())
            limpio += c;
        else
            limpio += '_';
    }
    if (limpio.isEmpty())
        limpio = "sin_nombre";

    return carpeta() + "/partida_" + limpio + ".bin";
}

// De cada carta se guarda el tipo y la vida
static void escribir_cartas(QDataStream &out, const vector<carta> &cartas)
{
    for (const carta &c : cartas)
        out << static_cast<int>(c.getTipo()) << c.get_vida();
}

// Lee 'cantidad' cartas. Devuelve false si los datos están mal
// o si no queda ninguna carta con vida (esa partida ya no se puede continuar).
static bool leer_cartas(QDataStream &in, int cantidad, vector<carta> &cartas)
{
    bool hay_viva = false;

    for (int i = 0; i < cantidad; i++) {
        int tipo = 0;
        int vida = 0;
        in >> tipo >> vida;

        if (in.status() != QDataStream::Ok || tipo < 0 || tipo > 3 || vida < 0 || vida > 100)
            return false;

        carta c(static_cast<TipoElemento>(tipo));
        c.setVida(vida);
        c.cambia_estado();      // si la vida es 0, la carta queda eliminada
        cartas.push_back(c);

        if (vida > 0)
            hay_viva = true;
    }
    return hay_viva;
}

// Lee un archivo de partida. Si está dañado o lo modificaron a mano devuelve false,
// así el juego no se rompe con datos incorrectos.
static bool leer_partida(const QString &ruta, partida_guardada &partida)
{
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly))
        return false;

    QDataStream in(&archivo);
    int cantidad = 0;
    in >> partida.nombre >> partida.dificultad >> cantidad;

    if (in.status() != QDataStream::Ok || partida.dificultad < 1 || partida.dificultad > 3
        || cantidad < 1 || cantidad > 50)
        return false;

    return leer_cartas(in, cantidad, partida.cartas_player)
        && leer_cartas(in, cantidad, partida.cartas_ia);
}

bool guardado::guardar(const partida_guardada &partida)
{
    QDir().mkpath(carpeta());       // crea la carpeta si no existe

    QFile archivo(ruta_archivo(partida.nombre));
    if (!archivo.open(QIODevice::WriteOnly))
        return false;

    // Orden en el archivo: nombre, dificultad, cantidad de cartas,
    // cartas del jugador y cartas de la IA
    QDataStream out(&archivo);
    out << partida.nombre << partida.dificultad << static_cast<int>(partida.cartas_player.size());
    escribir_cartas(out, partida.cartas_player);
    escribir_cartas(out, partida.cartas_ia);

    return out.status() == QDataStream::Ok;
}

vector<partida_guardada> guardado::listar()
{
    vector<partida_guardada> partidas;

    QDir dir(carpeta());
    QStringList archivos = dir.entryList(QStringList() << "partida_*.bin", QDir::Files);

    for (const QString &nombre_archivo : archivos) {
        partida_guardada p;
        if (leer_partida(dir.filePath(nombre_archivo), p))
            partidas.push_back(p);
    }
    return partidas;
}
