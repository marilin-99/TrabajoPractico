#include "partida.h"
#include "ui_partida.h"
#include <QMessageBox>
#include <QApplication>
#include <QGridLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QMessageBox>

partida::partida(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::partida)
{
    ui->setupUi(this);
    sonidoCarta.setSource(QUrl("qrc:/disenios/carta.wav"));
    sonidoCarta.setVolume(0.2);
    this->reglas = new class reglas(this);
    this->tabla = new tabla_danio_ventana(&this->juego, this);

    QGridLayout *layout = new QGridLayout(ui->centralwidget);

    // fila 0: botones arriba a la derecha
    QHBoxLayout *botonesLayout = new QHBoxLayout;
    botonesLayout->addWidget(ui->tabla_danio);
    botonesLayout->addWidget(ui->reglas);

    ui->tabla_danio->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->reglas->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->tabla_danio->setFixedSize(160, 50);
    ui->reglas->setFixedSize(160, 50);
    layout->addLayout(botonesLayout, 0, 5, 1, 2, Qt::AlignRight);

    // fila 1: spacer
    layout->addItem(new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding), 1, 0, 1, 7);

    // Carta 1 + barra
    QVBoxLayout *carta1Layout = new QVBoxLayout;
    carta1Layout->addWidget(ui->carta_1, 0, Qt::AlignCenter);
    carta1Layout->addWidget(ui->vidacarta_1, 0, Qt::AlignCenter);

    // Carta 2 + barra
    QVBoxLayout *carta2Layout = new QVBoxLayout;
    carta2Layout->addWidget(ui->carta_2, 0, Qt::AlignCenter);
    carta2Layout->addWidget(ui->vidacarta_2, 0, Qt::AlignCenter);

    // Carta 3 + barra
    QVBoxLayout *carta3Layout = new QVBoxLayout;
    carta3Layout->addWidget(ui->carta_3, 0, Qt::AlignCenter);
    carta3Layout->addWidget(ui->vidacarta_3, 0, Qt::AlignCenter);

    // Carta 4 + barra
    QVBoxLayout *carta4Layout = new QVBoxLayout;
    carta4Layout->addWidget(ui->carta_4, 0, Qt::AlignCenter);
    carta4Layout->addWidget(ui->vidacarta_4, 0, Qt::AlignCenter);

    // Carta 5 + barra
    QVBoxLayout *carta5Layout = new QVBoxLayout;
    carta5Layout->addWidget(ui->carta_5, 0, Qt::AlignCenter);
    carta5Layout->addWidget(ui->vidacarta_5, 0, Qt::AlignCenter);

    // Agregar cada bloque al grid en la misma fila
    layout->addLayout(carta1Layout, 5, 1);
    layout->addLayout(carta2Layout, 5, 2);
    layout->addLayout(carta3Layout, 5, 3);
    layout->addLayout(carta4Layout, 5, 4);
    layout->addLayout(carta5Layout, 5, 5);

    ui->carta_ia->setFixedSize(200, 300);
    layout->addWidget(ui->carta_ia, 2, 3, Qt::AlignCenter);

    layout->addItem(new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding), 7, 0, 1, 7);

    // ajustar ancho de las barras al ancho de cada carta
    ui->vidacarta_1->setFixedWidth(ui->carta_1->width());
    ui->vidacarta_2->setFixedWidth(ui->carta_2->width());
    ui->vidacarta_3->setFixedWidth(ui->carta_3->width());
    ui->vidacarta_4->setFixedWidth(ui->carta_4->width());
    ui->vidacarta_5->setFixedWidth(ui->carta_5->width());

    ui->centralwidget->setLayout(layout);

}

partida::~partida()
{
    delete ui;
}

void partida::mostrar_carta_ia()
{
    carta *carta_ia = this->juego.mostrar_carta_ia();
    int tipo_ia = static_cast<int>(carta_ia->getTipo());

    QPixmap imagen(":/disenios/" + QString::number(tipo_ia) + ".png");

    ui->carta_ia->setPixmap(
        imagen.scaled(ui->carta_ia->size(),
                      Qt::KeepAspectRatio,
                      Qt::SmoothTransformation)
        );

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(ui->carta_ia);
    ui->carta_ia->setGraphicsEffect(effect);

    QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(300);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    sonidoCarta.play();
}

void partida::habilitar_vivas()
{
    QPushButton *botones[] = {
        ui->carta_1,
        ui->carta_2,
        ui->carta_3,
        ui->carta_4,
        ui->carta_5
    };

    vector <carta> *cartas_player = this->juego.getcartas_player();
    for(int i=0; i<5; i++){
        if(cartas_player->at(i).getEstado()==true){
            botones[i]->setEnabled(1);
        }
    }
}

void partida::finalizar_partida()
{
    int resultado = this->juego.validar_partida();
    if (resultado == 1)
    {
        QMessageBox msg(this);

        msg.setWindowTitle("Fin de la partida");
        msg.setText("¡La IA ganó la partida!");
        msg.setIconPixmap(QPixmap(":/disenios/derrota.png"));
        msg.setStyleSheet(R"(
    QMessageBox {
        background-color: #140707;
        border: 3px solid #f59e0b;
    }

    QLabel {
        color: white;
        font-size: 18px;
    }

QMessageBox QPushButton {

    background: qlineargradient(
        x1:0, y1:0,
        x2:0, y2:1,
        stop:0 #22c55e,
        stop:1 #166534
    );

    color: white;

    border: 2px solid #fde68a;
    border-radius: 12px;

    min-width: 120px;
    min-height: 45px;

    font-size: 17px;
    font-weight: bold;
}
)");

        msg.exec();
        this->hide();
        this->parentWidget()->showMaximized();
    }
    else if (resultado == 2)
    {
        QMessageBox msg(this);

        msg.setWindowTitle("¡Victoria!");
        msg.setText("¡Felicitaciones! Ganaste la partida :D");
        msg.setIconPixmap(QPixmap(":/disenios/trofeo.png"));
        msg.setStyleSheet(R"(
    QMessageBox {

        background: qlineargradient(
            x1:0, y1:0,
            x2:0, y2:1,
            stop:0 #064e3b,
            stop:1 #022c22
        );

        border: 3px solid #facc15;
        border-radius: 18px;

    }


    QMessageBox QLabel {

        color: #fef3c7;

        font-size: 20px;

        font-weight: bold;

    }


QMessageBox QPushButton {

    background: qlineargradient(
        x1:0, y1:0,
        x2:0, y2:1,
        stop:0 #22c55e,
        stop:1 #166534
    );

    color: white;

    border: 2px solid #fde68a;
    border-radius: 12px;

    min-width: 120px;
    min-height: 45px;

    font-size: 17px;
    font-weight: bold;
}


    QMessageBox QPushButton:hover {

        background: qlineargradient(
            x1:0, y1:0,
            x2:0, y2:1,
            stop:0 #4ade80,
            stop:1 #15803d
        );

        border: 2px solid white;

    }


    QMessageBox QPushButton:pressed {

        background: #14532d;

    }

)");

        msg.exec();
        this->hide();
        this->parentWidget()->showMaximized();
    }
}

void partida::reiniciar_cartas()
{
    this->juego.reiniciar_cartas();
    this->actualizar_vida();
    this->habilitar_vivas();
}


void partida::asignar_cartas()
{
    this->juego.repartir_cartas();

    vector<carta> *cartas = this->juego.getcartas_player();

    QPushButton *botones[] = {
        ui->carta_1,
        ui->carta_2,
        ui->carta_3,
        ui->carta_4,
        ui->carta_5
    };

    this->juego.tabla_danio_ia();

    for (int i = 0; i < 5; i++) {
        int tipo = static_cast<int>(cartas->at(i).getTipo());

        QString ruta = ":/disenios/" + QString::number(tipo) + ".png";
        QPixmap imagen(ruta);

        botones[i]->setIcon(QIcon(imagen));
        botones[i]->setIconSize(botones[i]->size());
        botones[i]->setEnabled(true);
    }

    ui->carta_ia->clear();
    actualizar_vida();
}

void partida::actualizar_vida()
{
    QProgressBar *vidas[] = {
        ui->vidacarta_1,
        ui->vidacarta_2,
        ui->vidacarta_3,
        ui->vidacarta_4,
        ui->vidacarta_5
    };

    for(int i=0; i<5; i++){
        vector <carta> *cartas_player = this->juego.getcartas_player();
        int vida = cartas_player->at(i).get_vida();
        vidas[i]->setValue((vida));
}
}


void partida::setDificultad_ia(int newDificultad_ia)
{
    this->juego.set_ia(newDificultad_ia);
}


void partida::jugar_carta(int index)
{
    deshabilitar_botones();
    vector<carta> *cartas_player = this->juego.getcartas_player();
    this->juego.carta_interfaz(&cartas_player->at(index));
    this->juego.turnos();
    actualizar_vida();
    habilitar_vivas();
    mostrar_carta_ia();
    finalizar_partida();
}



void partida::deshabilitar_botones()
{
    this->ui->carta_1->setEnabled(0);
    this->ui->carta_2->setEnabled(0);
    this->ui->carta_3->setEnabled(0);
    this->ui->carta_4->setEnabled(0);
    this->ui->carta_5->setEnabled(0);
}





void partida::on_carta_1_clicked()
{
    jugar_carta(0);
}


void partida::on_carta_2_clicked()
{
    jugar_carta(1);
}

void partida::on_carta_3_clicked()
{
    jugar_carta(2);
}


void partida::on_carta_4_clicked()
{
    jugar_carta(3);
}


void partida::on_carta_5_clicked()
{
    jugar_carta(4);
}


void partida::on_reglas_clicked()
{
    this->reglas->show();
}


void partida::on_tabla_danio_clicked()
{
    this->tabla->show();
}



