#include "PageResultats.h"
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>
#include "SocketManager.h"
#include "Logger.h"

// Declare socketManager comme une instance globale
extern SocketManager socketManager;

// Constructeur de la classe PageResultats
PageResultats::PageResultats(int score, QWidget *parent) : QWidget(parent) {
    // Configuration de la taille de la fenetre
    resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Affichage du score final
    scoreLabel = new QLabel("Score final : " + QString::number(score), this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("font-size: 32px; font-weight: bold; font-family: Arial Black;");
    layout->addWidget(scoreLabel);

    Logger::log("[PageResultats] ligne 25 : Score final affiche : " + std::to_string(score));

    // Bouton pour retourner a l'accueil
    accueilButton = new QPushButton("Retour a l'accueil", this);
    accueilButton->setFixedHeight(60); // Hauteur du bouton
    accueilButton->setStyleSheet("font-size: 30px; font-weight: bold; font-family: Arial Black;");
    layout->addWidget(accueilButton);

    Logger::log("[PageResultats] ligne 33 : Bouton 'Retour a l'accueil' initialise");

    // Connexion du bouton "Retour a l'accueil" au slot correspondant
    connect(accueilButton, &QPushButton::clicked, this, &PageResultats::onRetourAccueilClicked);

    Logger::log("[PageResultats] ligne 38 : Signal de retour a l'accueil connecte");
}

// Slot pour gerer le retour a la page d'accueil
void PageResultats::onRetourAccueilClicked() {
    // Envoi du message "retour_accueil" au serveur
    QJsonObject message = {{"type", "retour_accueil"}};
    socketManager.envoyerMessage(message);

    Logger::log("[PageResultats] ligne 47 : Message 'retour_accueil' envoye au serveur MDJ");

    // Emet le signal pour informer l'application principale
    emit retourAccueil();
    Logger::log("[PageResultats] ligne 51 : Signal 'retourAccueil' emis");
}
