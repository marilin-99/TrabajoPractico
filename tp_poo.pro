QT += core gui widgets
TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
QT += multimedia

SOURCES += \
        carta.cpp \
        formato_partida.cpp \
        guardado.cpp \
        ia.cpp \
        ia_aleatoria.cpp \
        ia_estrategica.cpp \
        ia_super.cpp \
        juego.cpp \
        jugador.cpp \
        main.cpp \
        mainwindow.cpp \
        partida.cpp \
        persona.cpp \
        reglas.cpp \
        tabla_danio_ventana.cpp

HEADERS += \
    carta.h \
    formato_partida.h \
    guardado.h \
    ia.h \
    ia_aleatoria.h \
    ia_estrategica.h \
    ia_super.h \
    juego.h \
    jugador.h \
    mainwindow.h \
    partida.h \
    persona.h \
    reglas.h \
    tabla_danio_ventana.h

FORMS += \
    mainwindow.ui \
    partida.ui \
    reglas.ui \
    tabla_danio_ventana.ui

RESOURCES += \
    recursos.qrc
