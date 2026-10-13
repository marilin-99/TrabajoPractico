#include "guardado.h"
#include <QDir>
#include <QSaveFile>
#include <QStandardPaths>
#include <ctime>

QString guardado::carpeta()
{
    // Carpeta de datos de la aplicación (en Windows: %APPDATA%/TrabajoPractico).
    const QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (base.isEmpty())
        return QString();
    return base + "/partidas";
}

bool guardado::guardar(const datos_partida &datos, QString *ruta, QString *error)
{
    auto fallo = [error](const QString &motivo) {
        if (error)
            *error = motivo;
        return false;
    };

    const QString dir = carpeta();
    if (dir.isEmpty())
        return fallo("No se encontró una carpeta donde guardar la partida.");

    if (!QDir().mkpath(dir))
        return fallo("No se pudo crear la carpeta " + dir);

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
        return fallo("No se pudo abrir " + destino + ": " + archivo.errorString());

    const qint64 largo = static_cast<qint64>(bytes.size());
    if (archivo.write(bytes.data(), largo) != largo || !archivo.commit())
        return fallo("No se pudo escribir " + destino + ": " + archivo.errorString());

    if (ruta)
        *ruta = QDir::toNativeSeparators(destino);
    return true;
}
