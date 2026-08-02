#ifndef TABLA_DANIO_VENTANA_H
#define TABLA_DANIO_VENTANA_H

#include <QMainWindow>
#include "juego.h"

namespace Ui {
class tabla_danio_ventana;
}

class tabla_danio_ventana : public QMainWindow
{
    Q_OBJECT

public:
    explicit tabla_danio_ventana(juego* tabla, QWidget *parent = nullptr);
    ~tabla_danio_ventana();

private:
    Ui::tabla_danio_ventana *ui;
};

#endif // TABLA_DANIO_VENTANA_H
