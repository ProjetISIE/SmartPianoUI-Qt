# Nom du projet et configuration
TEMPLATE = app
TARGET = IHM
CONFIG += console c++17 qt debug_and_release

# Inclusion des modules Qt n�cessaires
QT += core gui network widgets

# R�pertoires des fichiers sources et d'en-t�te
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

# Fichiers g�n�r�s par le Meta-Object Compiler (MOC)
HEADERS += \
    include/PageAccueil.h \
    include/PageJeu.h \
    include/PageResultats.h \
    include/Portee.h \
    include/SocketManager.h

# Fichiers d'interface utilisateur (si utilis�s, ajoutez les .ui ici)
FORMS +=

# Fichiers de ressources (si n�cessaires, ajoutez les .qrc ici)
RESOURCES +=

# Configuration pour les fichiers d'installation
#INSTALLS += target

# R�pertoires de sortie
DESTDIR = .
#OBJECTS_DIR = ./build
#MOC_DIR = ./build/moc
#RCC_DIR = ./build/rcc
#UI_DIR = ./build/ui