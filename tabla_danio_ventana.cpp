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

    // Estilo extra: se suma al que ya existe
    ui->tablaDanio->setStyleSheet(ui->tablaDanio->styleSheet() +
                                  "QTableWidget {"
                                  "  background-color: #fff3d6;"
                                  "  gridline-color: #c9a566;"
                                  "  border: 3px solid #7a4a00;"
                                  "  font-size: 28px;"
                                  "  font-weight: bold;"
                                  "  color: #4a2c00;"
                                  "}"
                                  "QHeaderView::section {"
                                  "  background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                  "                    stop:0 #8d5a1a, stop:1 #5a3408);"
                                  "  color: #fff3d6;"
                                  "  font-size: 22px;"
                                  "  border: 1px solid #3b2300;"
                                  "  padding: 6px;"
                                  "}"
                                  "QTableCornerButton::section {"
                                  "  background-color: #5a3408;"
                                  "  border: 1px solid #3b2300;"
                                  "}"
                                  );

    // Centrar y colorear cada celda según el daño
    for (int i = 0; i < ui->tablaDanio->rowCount(); i++) {
        for (int j = 0; j < ui->tablaDanio->columnCount(); j++) {
            QTableWidgetItem *item = ui->tablaDanio->item(i, j);
            item->setTextAlignment(Qt::AlignCenter);

            int danio = item->text().toInt();
            if (danio >= 50)      item->setBackground(QColor("#a5d6a7"));
            else if (danio >= 35) item->setBackground(QColor("#dcedc8"));
            else if (danio >= 20) item->setBackground(QColor("#fff59d"));
            else                  item->setBackground(QColor("#ef9a9a"));
        }
    }

    this->resize(900, 700);


}

tabla_danio_ventana::~tabla_danio_ventana()
{
    delete ui;
}
