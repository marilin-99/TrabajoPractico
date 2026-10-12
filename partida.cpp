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
#include <QVBoxLayout>
#include <QFileDialog>


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

    // Botones superiores
    QHBoxLayout *botonesLayout = new QHBoxLayout;
    botonesLayout->addWidget(ui->Guardar);
    botonesLayout->addWidget(ui->Cargar);
    botonesLayout->addWidget(ui->tabla_danio);
    botonesLayout->addWidget(ui->reglas);

    ui->Guardar->setFixedSize(160, 50);
    ui->Cargar->setFixedSize(160, 50);
    ui->tabla_danio->setFixedSize(160, 50);
    ui->reglas->setFixedSize(160, 50);

    layout->addLayout(botonesLayout, 0, 0, 1, 7,
                      Qt::AlignRight);

    // Carta de la IA
    ui->carta_ia->setFixedSize(200, 300);
    layout->addWidget(ui->carta_ia, 2, 0, 1, 7,
                      Qt::AlignCenter);

    // Layout dinámico para las cartas del jugador
    layoutCartas = new QGridLayout;
    layout->addLayout(layoutCartas, 5, 0, 1, 7);

    // Espacio inferior
    layout->setRowStretch(7, 1);

    ui->centralwidget->setLayout(layout);
}


partida::~partida()
{
    delete ui;
}



void partida::crearBotonesCartas()
{
    // Quitar los layouts de las cartas anteriores
    while (QLayoutItem *item = layoutCartas->takeAt(0)) {
        delete item;
    }

    // Eliminar los botones y las barras anteriores
    for (QPushButton *boton : botonesCartas) {
        delete boton;
    }

    for (QProgressBar *barra : barrasVida) {
        delete barra;
    }

    botonesCartas.clear();
    barrasVida.clear();

    for (int i = 0; i < cant_cartas; i++) {
        QPushButton *boton = new QPushButton(ui->centralwidget);
        QProgressBar *barra = new QProgressBar(ui->centralwidget);

        boton->setFixedSize(120, 180);
        boton->setIconSize(QSize(120, 180));

        barra->setRange(0, 100);
        barra->setValue(100);
        barra->setFixedWidth(120);

        QVBoxLayout *cartaLayout = new QVBoxLayout;
        cartaLayout->addWidget(boton, 0, Qt::AlignCenter);
        cartaLayout->addWidget(barra, 0, Qt::AlignCenter);

        int fila = i / 5;
        int columna = i % 5;

        layoutCartas->addLayout(cartaLayout, fila, columna, 1, 1);

        botonesCartas.push_back(boton);
        barrasVida.push_back(barra);

        connect(boton, &QPushButton::clicked, this, [this, i]() {
            jugar_carta(i);
        });
    }
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
    vector<carta> *cartas = this->juego.getcartas_player();

    for (int i = 0; i < cant_cartas; i++) {
        botonesCartas[i]->setEnabled(
            cartas->at(i).getEstado()
            );
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



void partida::asignar_cartas()
{
    this->juego.setCant_cartas(cant_cartas);

    crearBotonesCartas();

    this->juego.repartir_cartas();
    this->juego.tabla_danio_ia();

    vector<carta> *cartas = this->juego.getcartas_player();

    for (int i = 0; i < cant_cartas; i++) {
        int tipo = static_cast<int>(cartas->at(i).getTipo());

        QString ruta = ":/disenios/" + QString::number(tipo) + ".png";
        QPixmap imagen(ruta);

        botonesCartas[i]->setIcon(QIcon(imagen));
        botonesCartas[i]->setEnabled(true);
    }

    ui->carta_ia->clear();

    actualizar_vida();
}



void partida::actualizar_vida()
{
    vector<carta> *cartas = this->juego.getcartas_player();

    for (int i = 0; i < cant_cartas; i++) {
        int vida = cartas->at(i).get_vida();
        barrasVida[i]->setValue(vida);
    }
}



void partida::setDificultad_ia(int newDificultad_ia)
{
    this->juego.set_ia(newDificultad_ia);
}


void partida::jugar_carta(int index)
{
    // Verificar que el índice sea válido.
    if (index < 0 || index >= botonesCartas.size()) {
        return;
    }

    vector<carta> *cartas_player = this->juego.getcartas_player();

    // No jugar una carta que ya está muerta.
    if (!cartas_player->at(index).getEstado()) {
        return;
    }

    deshabilitar_botones();

    this->juego.carta_interfaz(&cartas_player->at(index));
    this->juego.turnos();

    actualizar_vida();
    mostrar_carta_ia();

    // Primero verificar si alguien gano
    finalizar_partida();

    // Si la partida continúa, habilitar las cartas vivas.
    habilitar_vivas();
}




void partida::deshabilitar_botones()
{
    for (QPushButton *boton : botonesCartas) {
        boton->setEnabled(false);
    }
}


void partida::on_reglas_clicked()
{
    this->reglas->show();
}


void partida::on_tabla_danio_clicked()
{
    this->tabla->show();
}

void partida::setCant_cartas(int newCant_cartas)
{
    cant_cartas = qBound(1, newCant_cartas, 50);
}


void partida::refrescar_interfaz()
{
    vector<carta> *cartas = this->juego.getcartas_player();

    cant_cartas = cartas->size();
    crearBotonesCartas();

    for (int i = 0; i < cant_cartas; i++) {
        int tipo = static_cast<int>(cartas->at(i).getTipo());
        botonesCartas[i]->setIcon(QIcon(":/disenios/" + QString::number(tipo) + ".png"));
    }

    ui->carta_ia->clear();
    actualizar_vida();
    habilitar_vivas();
}

void partida::on_Guardar_clicked()
{
    QString ruta = QFileDialog::getSaveFileName(this, "Guardar partida", "", "Partida (*.dat)");
    if (ruta.isEmpty()) return;

    if (juego.guardar_partida(ruta.toStdString()))
        QMessageBox::information(this, "Guardado", "Partida guardada.");
    else
        QMessageBox::warning(this, "Error", "No se pudo guardar la partida.");
}

void partida::on_Cargar_clicked()
{
    QString ruta = QFileDialog::getOpenFileName(this, "Cargar partida", "", "Partida (*.dat)");
    if (ruta.isEmpty()) return;

    if (!juego.cargar_partida(ruta.toStdString())) {
        QMessageBox::warning(this, "Error", "Archivo inválido o dañado.");
        return;
    }
    refrescar_interfaz();
}

