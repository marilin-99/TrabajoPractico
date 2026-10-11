#include "tabla_danio_ventana.h"
#include "ui_tabla_danio_ventana.h"

tabla_danio_ventana::tabla_danio_ventana(juego* tabla, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::tabla_danio_ventana)
{
    ui->setupUi(this);


    auto matriz = tabla->getTabla_danio();

    ui->tablaDanio->setRowCount(matriz.size());
    ui->tablaDanio->setColumnCount(matriz[0].size());

    QStringList encabezados;
    encabezados << "Fuego" << "Agua" << "Tierra"<< "Viento";
    ui->tablaDanio->setHorizontalHeaderLabels(encabezados);
    ui->tablaDanio->setVerticalHeaderLabels(encabezados);

    for (size_t i = 0; i < matriz.size(); i++) {
        for (size_t j = 0; j < matriz[i].size(); j++) {
            ui->tablaDanio->setItem(i, j, new QTableWidgetItem(QString::number(matriz[i][j])));
        }
    }


    ui->tablaDanio->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->tablaDanio->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablaDanio->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    ui->tablaDanio->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tablaDanio->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    ui->tablaDanio->setShowGrid(true);
    ui->tablaDanio->setGridStyle(Qt::SolidLine);
    ui->tablaDanio->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tablaDanio->setFocusPolicy(Qt::NoFocus);
    ui->tablaDanio->setStyleSheet(
        "QTableWidget { font-size: 60px; gridline-color: black; } "
        "QHeaderView::section { font-size: 40px; font-weight: bold; border: 1px solid black; }"
        );


    this->resize(900, 700);


}

tabla_danio_ventana::~tabla_danio_ventana()
{
    delete ui;
}
