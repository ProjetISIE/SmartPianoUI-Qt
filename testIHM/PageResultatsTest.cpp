#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/PageResultats.h"
#include "../include/SocketManager.h"
#include <QApplication>
#include <QSignalSpy>
#include <QTest>


// Simulation du socketManager global
SocketManager socketManager;

// Test de l'initialisation de PageResultats
TEST_CASE("PageResultats - Initialisation") {
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    PageResultats page(95);
    CHECK(page.isVisible() == false);
}

// Test de l'affichage du score
TEST_CASE("PageResultats - Affichage du score") {
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    PageResultats page(85);
    CHECK(page.findChild<QLabel *>()->text() == "Score final : 85");
}

