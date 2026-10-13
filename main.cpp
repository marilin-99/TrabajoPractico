#include "mainwindow.h"
#include "partida.h"
#include "juego.h"
#include <QApplication>
#include <cstdlib>
#include <ctime>



int main(int argc, char *argv[])
{
    // Semilla única para todo el programa (reparto de cartas e IA aleatoria)
    srand(static_cast<unsigned>(time(nullptr)));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
