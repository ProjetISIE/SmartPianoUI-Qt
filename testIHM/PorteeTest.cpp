#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/Portee.h"
#include <QApplication>
#include <QTest>

// Test de l'initialisation de la portee
TEST_CASE("Portee - Initialisation") {
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    Portee portee;
    CHECK(portee.isVisible() == false);
}

// Test de l'affichage d'une note
TEST_CASE("Portee - Affichage d'une note") {
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    Portee portee;
    portee.setNoteToDisplay("C4");

    CHECK(portee.getNoteToDisplay() == "C4");
}

// Test du rafraichissement de l'affichage
TEST_CASE("Portee - Rafraichissement de l'affichage") {
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    Portee portee;
    portee.setNoteToDisplay("G4");

    QTest::qWait(500);
    CHECK(portee.getNoteToDisplay() == "G4");
}
