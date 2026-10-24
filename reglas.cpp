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
    ui->ReglasJuego->setStyleSheet(
        "QLabel {"
        "  background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                    stop:0 #fff3d6, stop:1 #ecd29a);"
        "  color: #4a2c00;"
        "  border: 3px solid #7a4a00;"
        "  border-radius: 16px;"
        "  padding: 18px 24px;"
        "  font-family: 'Palatino Linotype', 'Book Antiqua', Georgia, serif;"
        "  font-size: 19px;"
        "}"
        );
    ui->ReglasJuego->setTextFormat(Qt::RichText);
    ui->ReglasJuego->setText(
        "<h2 style='color:#7a1d0a;'>⚔ Reglas del juego</h2>"
        "<p><b>Elementos:</b> "
        "<span style='color:#c62828;'>Fuego</span>, "
        "<span style='color:#1565c0;'>Agua</span>, "
        "<span style='color:#6d4c1f;'>Tierra</span> y "
        "<span style='color:#2e7d32;'>Viento</span>.</p>"
        "<p><b>1.</b> El jugador elige la IA a la que se va a enfrentar.</p>"
        "<p><b>2.</b> Se reparten cartas al azar.</p>"
        "<p><b>3.</b> El jugador elige qué carta se enfrenta a la de su oponente.</p>"
        "<p><b>4.</b> Cada enfrentamiento resta vida a la carta (según la tabla de daño).</p>"
        "<p><b>5.</b> Cuando la vida de una carta llega a 0, ya no puede usarse.</p>"
        "<p><b>6.</b> Gana quien termina la partida con al menos 1 carta con vida.</p>"
        );
    ui->ReglasJuego->setStyleSheet(ui->ReglasJuego->styleSheet() +
                                   "QLabel { font-family: 'Palatino Linotype', 'Book Antiqua', Georgia, serif; }"
                                   );
}

reglas::~reglas()
{
    delete ui;
}
