#include "partida.h"
#include "ui_partida.h"
#include "guardado.h"
#include <QMessageBox>
#include <QApplication>
#include <QGridLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QMessageBox>
#include <QVBoxLayout>


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
    botonesLayout->addWidget(ui->tabla_danio);
    botonesLayout->addWidget(ui->reglas);

    ui->tabla_danio->setFixedSize(160, 50);
    ui->reglas->setFixedSize(160, 50);

    layout->addLayout(botonesLayout, 0, 0, 1, 7,
                      Qt::AlignRight);

    // Cartas de la IA
    layoutCartasIA = new QGridLayout;
    layout->addLayout(layoutCartasIA, 3, 0, 1, 7);

    // Carta que acaba de jugar la IA
    ui->carta_ia->setFixedSize(120, 180);
    layout->addWidget(ui->carta_ia, 6, 0, 1, 7,
                      Qt::AlignCenter);

    // Cartas del jugador
    layoutCartas = new QGridLayout;
    layout->addLayout(layoutCartas, 8, 0, 1, 7);

    // Espacio inferior
    layout->setRowStretch(7, 1);

    ui->centralwidget->setLayout(layout);
}


partida::~partida()
{
    delete ui;
}



void partida::crearCartasIA()
{
    // Eliminar los elementos anteriores del layout
    while (QLayoutItem *item = layoutCartasIA->takeAt(0)) {
        delete item;
    }

    for (QLabel *imagen : imagenesCartasIA)
        delete imagen;

    for (QProgressBar *barra : barrasVidaIA)
        delete barra;

    imagenesCartasIA.clear();
    barrasVidaIA.clear();

    for (int i = 0; i < cant_cartas; i++) {
        QLabel *imagen = new QLabel(ui->centralwidget);
        QProgressBar *barra = new QProgressBar(ui->centralwidget);

        imagen->setFixedSize(80, 120);
        imagen->setAlignment(Qt::AlignCenter);
        imagen->setScaledContents(false);

        barra->setRange(0, 100);
        barra->setValue(100);
        barra->setFixedWidth(80);
        barra->setTextVisible(true);

        QVBoxLayout *cartaLayout = new QVBoxLayout;
        cartaLayout->addWidget(imagen, 0, Qt::AlignCenter);
        cartaLayout->addWidget(barra, 0, Qt::AlignCenter);

        int fila = i / 5;
        int columna = i % 5;

        layoutCartasIA->addLayout(cartaLayout, fila, columna);

        imagenesCartasIA.push_back(imagen);
        barrasVidaIA.push_back(barra);
    }
}


void partida::actualizarVidaIA()
{
    vector<carta> *cartas = juego.getcartas_ia();

    for (int i = 0; i < cant_cartas; i++) {
        int tipo = static_cast<int>(cartas->at(i).getTipo());

        QPixmap imagen(
            ":/disenios/" + QString::number(tipo) + ".png"
            );

        imagenesCartasIA[i]->setPixmap(
            imagen.scaled(
                imagenesCartasIA[i]->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                )
            );

        barrasVidaIA[i]->setValue(cartas->at(i).get_vida());

        // Diferenciar visualmente las cartas eliminadas
        if (!cartas->at(i).getEstado()) {
            imagenesCartasIA[i]->setStyleSheet(
                "background-color: #444444;"
                );
            imagenesCartasIA[i]->setGraphicsEffect(nullptr);
        } else {
            imagenesCartasIA[i]->setStyleSheet(
                "background-color: transparent;"
                );
        }
    }
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

        boton->setFixedSize(120, 165);
        boton->setIconSize(QSize(120, 180));

        boton->setStyleSheet("background-color: transparent; border: none;");

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


    //si el resultado es 3
    else if(resultado == 3) {
        QMessageBox msg(this);

        msg.setWindowTitle("¡Empateee!");
        msg.setText("¡Nadie gana! Empataste la partida :/");
        msg.setIconPixmap(QPixmap(":/disenios/empate.png"));
        msg.setStyleSheet(R"(
    QMessageBox {
        background: qlineargradient(
            x1:0, y1:0,
            x2:0, y2:1,
            stop:0 #1e3a8a, /* Azul oscuro profundo */
            stop:1 #172554  /* Azul marino */
        );
        border: 3px solid #60a5fa; /* Borde azul claro */
        border-radius: 18px;
    }

    QMessageBox QLabel {
        color: #eff6ff; /* Blanco con un ligero tono azul */
        font-size: 20px;
        font-weight: bold;
    }

    QMessageBox QPushButton {
        background: qlineargradient(
            x1:0, y1:0,
            x2:0, y2:1,
            stop:0 #3b82f6, /* Azul vibrante */
            stop:1 #1d4ed8  /* Azul medio oscuro */
        );
        color: white;
        border: 2px solid #bfdbfe; /* Borde celeste muy claro */
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
            stop:0 #60a5fa, /* Azul más claro al pasar el cursor */
            stop:1 #2563eb  /* Azul vibrante */
        );
        border: 2px solid white;
    }

    QMessageBox QPushButton:pressed {
        background: #1e40af; /* Azul oscuro al hacer clic */
    }
)");

        msg.exec();
        this->hide();
        this->parentWidget()->showMaximized();
    }

}






void partida::asignar_cartas()
{
    juego.setCant_cartas(cant_cartas);

    crearBotonesCartas();
    crearCartasIA();

    juego.repartir_cartas();
    juego.tabla_danio_ia();

    cargar_iconos_cartas();
    habilitar_vivas();   // al empezar todas están vivas

    ui->carta_ia->clear();

    actualizar_vida();
    actualizarVidaIA();
}


// Pone en cada botón la imagen del elemento de su carta
void partida::cargar_iconos_cartas()
{
    vector<carta> *cartas = juego.getcartas_player();

    for (int i = 0; i < cant_cartas; i++) {
        int tipo = static_cast<int>(cartas->at(i).getTipo());

        QPixmap imagen(
            ":/disenios/" + QString::number(tipo) + ".png"
            );

        botonesCartas[i]->setIcon(QIcon(imagen));
    }
}


void partida::cargar_partida(const partida_guardada &guardada)
{
    setDificultad_ia(guardada.dificultad);      // primero la IA, porque las cartas se le asignan después
    setCant_cartas(guardada.cartas_player.size());
    setNombre(guardada.nombre);

    crearBotonesCartas();
    crearCartasIA();

    juego.restaurar_cartas(guardada.cartas_player, guardada.cartas_ia);
    juego.tabla_danio_ia();

    cargar_iconos_cartas();
    ui->carta_ia->clear();

    actualizar_vida();
    actualizarVidaIA();
    habilitar_vivas();      // las cartas sin vida quedan deshabilitadas
}

void partida::aplicar_configuracion(const Configuracion &config)
{
    this->juego.aplicar_configuracion(config);
    delete this->tabla;
    this->tabla = new tabla_danio_ventana(&this->juego, this);
}

void partida::mostrar_danios()
{
    int danio_ia = this->juego.mostrar_danios_ia();
    int danio_player = this->juego.mostrar_danios_player();

    ui->mostrador_danio->setText("Hiciste " + QString::number(danio_player) + " de daño \nLa IA te hizo " + QString::number(danio_ia) + " de daño");
    ui->mostrador_danio->setAlignment(Qt::AlignCenter);
    ui->mostrador_danio->setWordWrap(true);
    ui->mostrador_danio->setMinimumSize(260, 90);
    ui->mostrador_danio->setStyleSheet(
        "QLabel {"
        "  background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                    stop:0 #2b2d42, stop:1 #14151f);"
        "  color: #f1f1f1;"
        "  border: 2px solid #ef233c;"
        "  border-radius: 12px;"
        "  padding: 10px;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "}"
        );

    auto *sombra = new QGraphicsDropShadowEffect(this);
    sombra->setBlurRadius(20);
    sombra->setOffset(0, 4);
    sombra->setColor(QColor(0, 0, 0, 160));
    ui->mostrador_danio->setGraphicsEffect(sombra);
    ui->mostrador_danio->setGeometry(940, 375, 280, 90);
    ui->mostrador_danio->move((width() - ui->mostrador_danio->width()) / 2, 270);
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
    deshabilitar_botones();

    vector<carta> *cartas_player = juego.getcartas_player();

    juego.carta_interfaz(&cartas_player->at(index));
    juego.turnos();

    actualizar_vida();
    actualizarVidaIA();
    mostrar_danios();

    habilitar_vivas();
    mostrar_carta_ia();
    finalizar_partida();
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

void partida::on_guardar_partida_clicked()
{
    partida_guardada guardada;
    guardada.nombre = this->nombre.trimmed();
    guardada.dificultad = this->juego.getDificultad();
    guardada.cartas_player = *this->juego.getcartas_player();
    guardada.cartas_ia = *this->juego.getcartas_ia();

    if (guardado::guardar(guardada))
        QMessageBox::information(this, "Partida guardada", "La partida se guardó correctamente.");
    else
        QMessageBox::warning(this, "Error", "No se pudo guardar la partida.");
}

void partida::setNombre(const QString &newNombre)
{
    this->nombre = newNombre;
    this->ui->nombre_player->setText(newNombre);
}

void partida::setCant_cartas(int newCant_cartas)
{
    cant_cartas = qBound(1, newCant_cartas, 50);
}




void partida::on_menu_clicked()
{
    this->hide();
    this->parentWidget()->showMaximized();
}

