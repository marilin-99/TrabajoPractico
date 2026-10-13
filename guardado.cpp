#include "guardado.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>
#include <algorithm>
#include <ctime>

QString guardado::carpeta()
{
    // Carpeta del ejecutable del juego + "/partidas"
    return QCoreApplication::applicationDirPath() + "/partidas";
}

bool guardado::guardar(const datos_partida &datos, QString *ruta, QString *error)
{
    auto fallo = [error](const QString &motivo) {
        if (error)
            *error = motivo;
        return false;
    };

    const QString dir = carpeta();
    if (!QDir().mkpath(dir))
        return fallo("No se pudo crear la carpeta " + QDir::toNativeSeparators(dir));

    // Si no se indicó, la fecha es el momento en que se guarda
    datos_partida copia = datos;
    if (copia.fecha == 0)
        copia.fecha = static_cast<std::int64_t>(std::time(nullptr));

    const std::string bytes = serializar_partida(copia);
    const QString destino = dir + "/" + QString::fromStdString(nombre_de_archivo(copia.nombre));

    // QSaveFile escribe a un archivo temporal y recién al final reemplaza el
    // original: si algo falla, no queda una partida guardada a medias.
    // (Se abre sin QIODevice::Text para que los bytes se escriban tal cual.)
    QSaveFile archivo(destino);
    if (!archivo.open(QIODevice::WriteOnly))
        return fallo("No se pudo abrir " + QDir::toNativeSeparators(destino) + ": " + archivo.errorString());

    const qint64 largo = static_cast<qint64>(bytes.size());
    if (archivo.write(bytes.data(), largo) != largo || !archivo.commit())
        return fallo("No se pudo escribir " + QDir::toNativeSeparators(destino) + ": " + archivo.errorString());

    if (ruta)
        *ruta = QDir::toNativeSeparators(destino);
    return true;
}

bool guardado::cargar(const QString &ruta, datos_partida &datos, QString *error)
{
    auto fallo = [error](const QString &motivo) {
        if (error)
            *error = motivo;
        return false;
    };

    // Se abre sin QIODevice::Text para leer los bytes tal cual
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly))
        return fallo("No se pudo abrir " + QDir::toNativeSeparators(ruta) + ": " + archivo.errorString());

    // Una partida ocupa unas pocas decenas de bytes: algo enorme no es una partida
    if (archivo.size() > 1024 * 1024)
        return fallo("El archivo es demasiado grande para ser una partida guardada.");

    const QByteArray bytes = archivo.readAll();
    std::string motivo;
    if (!leer_partida(std::string(bytes.constData(), static_cast<size_t>(bytes.size())), datos, motivo))
        return fallo(QString::fromStdString(motivo));

    return true;
}

std::vector<partida_guardada> guardado::listar(int *danadas)
{
    std::vector<partida_guardada> partidas;
    int malas = 0;

    const QDir dir(carpeta());
    const QFileInfoList archivos =
        dir.entryInfoList(QStringList() << "partida_*.bin", QDir::Files, QDir::Name);

    for (const QFileInfo &info : archivos) {
        partida_guardada p;
        p.ruta = info.absoluteFilePath();
        if (cargar(p.ruta, p.datos, nullptr))
            partidas.push_back(p);
        else
            malas++;
    }

    // La más reciente primero
    std::sort(partidas.begin(), partidas.end(),
              [](const partida_guardada &a, const partida_guardada &b) {
                  return a.datos.fecha > b.datos.fecha;
              });

    if (danadas)
        *danadas = malas;
    return partidas;
}
