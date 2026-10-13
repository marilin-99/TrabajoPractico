#include "dialogo_cargar.h"
#include <QDateTime>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>

static QString nombre_dificultad(int dificultad)
{
    switch (dificultad) {
    case 1: return "IA Aleatoria";
    case 2: return "IA Estratégica";
    case 3: return "IA Super";
    }
    return "IA";
}

dialogo_cargar::dialogo_cargar(const std::vector<partida_guardada> &partidas,
                               int danadas,
                               QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Cargar partida");
    setMinimumWidth(560);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Elegí la partida que querés continuar:"));

    lista = new QListWidget;
    for (const partida_guardada &p : partidas) {
        QString nombre = QString::fromStdString(p.datos.nombre);
        if (nombre.isEmpty())
            nombre = "(sin nombre)";

        const QString fecha = QDateTime::fromSecsSinceEpoch(p.datos.fecha)
                                  .toString("dd/MM/yyyy HH:mm");

        lista->addItem(nombre + "  -  " +
                       nombre_dificultad(p.datos.dificultad_ia) + "  -  " +
                       QString::number(static_cast<int>(p.datos.cartas_player.size())) +
                       " cartas  -  " + fecha);
    }
    layout->addWidget(lista);

    if (danadas > 0) {
        layout->addWidget(new QLabel(
            "No se pudieron leer " + QString::number(danadas) +
            " archivo(s) de la carpeta (dañados o de otra versión)."));
    }

    QDialogButtonBox *botones = new QDialogButtonBox;
    boton_cargar = botones->addButton("Cargar", QDialogButtonBox::AcceptRole);
    botones->addButton("Cancelar", QDialogButtonBox::RejectRole);
    boton_cargar->setEnabled(false);
    layout->addWidget(botones);

    connect(botones, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(botones, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // "Cargar" solo se puede apretar si hay una partida seleccionada
    connect(lista, &QListWidget::currentRowChanged, this, [this](int fila) {
        boton_cargar->setEnabled(fila >= 0);
    });
    // Doble clic sobre una partida = cargarla
    connect(lista, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *) {
        accept();
    });

    lista->setCurrentRow(0);   // la más reciente queda seleccionada
}

int dialogo_cargar::elegida() const
{
    return lista->currentRow();
}
