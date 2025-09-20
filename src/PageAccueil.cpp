#include "PageAccueil.h"
#include "PageJeu.h"
#include <iostream>

// Constructeur de la classe PageAccueil
PageAccueil::PageAccueil(QWidget *parent) : QWidget(parent)
{
    // Creation d'un layout vertical pour la disposition des widgets
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Style global pour les polices et widgets
    QFont police("Arial Black", 22, QFont::Bold);
    this->setFont(police);

    // Espacement des widgets
    layout->setSpacing(20);
    layout->setContentsMargins(20, 20, 20, 20);

    // Selection du type de jeu
    QLabel *jeuLabel = new QLabel("Selectionnez un type de jeu :", this);
    jeuLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(jeuLabel);

    jeuBox = new QComboBox(this);
    jeuBox->addItems({"Jeu de note", "Jeu d'accord SR", "Jeu d'accord AR"});
    jeuBox->setFixedHeight(40);
    layout->addWidget(jeuBox);

    // Selection de la gamme
    QLabel *gammeLabel = new QLabel("Selectionnez une gamme :", this);
    gammeLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(gammeLabel);

    gammeBox = new QComboBox(this);
    gammeBox->addItems({"Do", "Ré", "Mi", "Fa", "Sol", "La", "Si"});
    gammeBox->setFixedHeight(40);
    layout->addWidget(gammeBox);

    // Selection du mode
    QLabel *modeLabel = new QLabel("Selectionnez un mode :", this);
    modeLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(modeLabel);

    modeBox = new QComboBox(this);
    modeBox->addItems({"Majeur", "Mineur"});
    modeBox->setFixedHeight(40);
    layout->addWidget(modeBox);

    // Option pour activer/desactiver le son
    QLabel *sonLabel = new QLabel("Activer le son :", this);
    sonLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(sonLabel);

    sonCheckBox = new QCheckBox(this);
    sonCheckBox->setChecked(true); // Par defaut, le son est active
    // Agrandir la checkbox avec un style personnalisé
    sonCheckBox->setStyleSheet("QCheckBox::indicator {"
                               "    width: 40px;"  // Largeur de la checkbox
                               "    height: 40px;" // Hauteur de la checkbox
                               "}");

    layout->addWidget(sonCheckBox);

    // Bouton pour lancer le jeu
    lancerButton = new QPushButton("Lancer le jeu", this);
    lancerButton->setFixedHeight(50);
    layout->addWidget(lancerButton);

    // Connexion du bouton "Lancer le jeu" au slot onLancerJeu
    connect(lancerButton, &QPushButton::clicked, this, &PageAccueil::onLancerJeu);

    this->setMinimumSize(800, 600);
}

// Slot appele lorsque l'utilisateur clique sur "Lancer le jeu"
void PageAccueil::onLancerJeu()
{
    QString jeu = jeuBox->currentText();      // Recupere le type de jeu selectionne
    QString gamme = gammeBox->currentText();  // Recupere la gamme selectionnee
    QString mode = modeBox->currentText();    // Recupere le mode selectionne
    bool jouerSon = sonCheckBox->isChecked(); // Recupere l'etat de la case a cocher
    std::cout << "[PageAccueil] Etat de jouerSon : "
              << (jouerSon ? "true" : "false") << std::endl;
    // Emet un signal avec les informations necessaires pour lancer le jeu
    emit lancerJeu(jeu, gamme, mode, jouerSon);
}