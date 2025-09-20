#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/PageJeu.h"
#include "../include/SocketManager.h"
#include <QApplication>
#include <QSignalSpy>
#include <QTest>

// Simulation du socketManager global
SocketManager socketManager;

// Test de l'initialisation de PageJeu
TEST_CASE("PageJeu - Initialisation") {
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    PageJeu page("Jeu de note", "Do", "Majeur", false);
    CHECK(page.isVisible() == false);
}


// Test de l'envoi de resultat
TEST_CASE("PageJeu - Envoi de resultat") {
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    PageJeu page("Jeu de note", "Do", "Majeur", false);

    // Simuler un envoi de resultat
    page.envoyerResultat(true);
}


