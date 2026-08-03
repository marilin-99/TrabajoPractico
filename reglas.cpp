#include "reglas.h"
#include "ui_reglas.h"
#include <QLabel>

reglas::reglas(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::reglas)
{
    ui->setupUi(this);

    ui->ReglasJuego->setText(
        "Elementos del juego: Agua, Tierra y Fuego.\n\n"
        "Reglas:\n\n"
        "1. El jugador elige la IA a la que se va a enfrentar.\n\n"
        "2. Se reparten 5 cartas al azar.\n\n"
        "3. El jugador debe elegir qué carta se enfrenta a la de su oponente.\n\n"
        "4. Cada enfrentamiento resta vida a la carta (como se representa en la tabla de daño).\n\n"
        "5. Cuando la vida de la carta llega a 0 esta ya no puede ser utilizada.\n\n"
        "6. Gana el jugador que termina la partida con al menos 1 carta con vida."
        );

    ui->ReglasJuego->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->ReglasJuego->setWordWrap(true); //
    ui->ReglasJuego->setStyleSheet("font-size: 18px; color: black;");

}

reglas::~reglas()
{
    delete ui;
}
