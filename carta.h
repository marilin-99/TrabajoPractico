#ifndef CARTA_H
#define CARTA_H

enum class TipoElemento { FUEGO, AGUA, TIERRA};

class carta
{
private:
    int vida;
    TipoElemento tipo;
    //int tipo;
    bool estado;
public:
    carta(TipoElemento tipo_);
    void cambia_estado(); //true si vida > 0
    int get_vida();
    void recibir_danio(int porcentaje); //resta vida.
    TipoElemento getTipo() const;
    bool getEstado() const;
};

#endif // CARTA_H