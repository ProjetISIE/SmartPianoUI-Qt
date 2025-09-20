#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/PageAccueil.h"
#include <QApplication>
#include <QSignalSpy>
#include <QTest>

// Test de l'initialisation de PageAccueil
TEST_CASE("PageAccueil - Initialisation") {
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    PageAccueil page;
    CHECK(page.isVisible() == false);
}

// Test de la selection des options
TEST_CASE("PageAccueil - Selection des options") {
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    PageAccueil page;

    // Selection d'un type de jeu
    page.getJeuBox()->setCurrentText("Jeu d'accord AR");
    CHECK(page.getJeuBox()->currentText() == "Jeu d'accord AR");

    // Selection d'une gamme
    page.getGammeBox()->setCurrentText("Sol");
    CHECK(page.getGammeBox()->currentText() == "Sol");

    // Selection d'un mode
    page.getModeBox()->setCurrentText("Mineur");
    CHECK(page.getModeBox()->currentText() == "Mineur");

    // Activer/desactiver le son
    page.getSonCheckBox()->setChecked(false);
    CHECK(page.getSonCheckBox()->isChecked() == false);
}