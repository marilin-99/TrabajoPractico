#ifndef REGLAS_H
#define REGLAS_H

#include <QMainWindow>

namespace Ui {
class reglas;
}

class reglas : public QMainWindow
{
    Q_OBJECT

public:
    explicit reglas(QWidget *parent = nullptr);
    ~reglas();

private:
    Ui::reglas *ui;
};

#endif // REGLAS_H
