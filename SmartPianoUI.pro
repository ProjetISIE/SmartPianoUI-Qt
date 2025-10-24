TEMPLATE = app
TARGET = SmartPianoUI
CONFIG += console c++23 qt debug_and_release
QT += core gui network widgets

INCLUDEPATH += include
SOURCES += \
    src/Main.cpp \
    src/PageAccueil.cpp \
    src/PageJeu.cpp \
    src/PageResultats.cpp \
    src/Portee.cpp \
    src/SocketManager.cpp
HEADERS += \
    include/PageAccueil.h \
    include/PageJeu.h \
    include/PageResultats.h \
    include/Portee.h \
    include/SocketManager.h \
    include/Logger.h

HEADERS += \
    include/PageAccueil.h \
    include/PageJeu.h \
    include/PageResultats.h \
    include/Portee.h \
    include/SocketManager.h

#INSTALLS += target
DESTDIR = .
#OBJECTS_DIR = ./build
#MOC_DIR = ./build/moc
#RCC_DIR = ./build/rcc
#UI_DIR = ./build/ui
