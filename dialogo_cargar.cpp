#include "dialogo_cargar.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

dialogo_cargar::dialogo_cargar(const vector<partida_guardada> &partidas, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Cargar partida");
    setMinimumWidth(560);

    QVBoxLayout *ventana_carga = new QVBoxLayout(this);
    ventana_carga->addWidget(new QLabel("Elegí la partida que querés continuar:"));

    // Una línea por partida: nombre - IA - cantidad de cartas
    const char *nombres_ia[] = {"", "IA Aleatoria", "IA Estratégica", "IA Super"};

    lista = new QListWidget;
    for (const partida_guardada &p : partidas) {
        QString nombre = p.nombre.isEmpty() ? "(sin nombre)" : p.nombre;

        lista->addItem(nombre + "  -  " + nombres_ia[p.dificultad] + "  -  " +
                       QString::number(p.cartas_player.size()) + " cartas");
    }
    lista->setCurrentRow(0);        // empieza con la primera seleccionada
    ventana_carga->addWidget(lista);

    QDialogButtonBox *botones = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    botones->button(QDialogButtonBox::Ok)->setText("Cargar");
    botones->button(QDialogButtonBox::Cancel)->setText("Cancelar");
    ventana_carga->addWidget(botones);

    connect(botones, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(botones, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(lista, &QListWidget::itemDoubleClicked, this, &QDialog::accept);   // doble clic = cargar
}

int dialogo_cargar::elegida() const
{
    return lista->currentRow();
}
