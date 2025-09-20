#include "PageJeu.h"
#include "Logger.h"
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <QProcess>
#include <QString>

// Constructeur de la classe PageJeu
PageJeu::PageJeu(const QString &jeu, const QString &gamme, const QString &mode, bool jouerSon, QWidget *parent)
    : QWidget(parent), enPause(false), score(0), jeuType(jeu)
{

    std::cout << "[IHM] Initialisation de la page de jeu." << std::endl;

    resize(1000, 800); // Definition de la taille initiale de la fenetre
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Configuration de l'affichage en fonction du type de jeu
    if (jeuType != "Jeu d'accord SR" && jeuType != "Jeu d'accord AR")
    {
        // Configuration pour le jeu de note
        QHBoxLayout *porteeLayout = new QHBoxLayout();
        portee = new Portee(this);
        portee->setMinimumSize(800, 400); // Taille minimale pour l'affichage des notes
        porteeLayout->addStretch();
        porteeLayout->addWidget(portee);
        porteeLayout->addStretch();
        mainLayout->addLayout(porteeLayout);
        Logger::log("[PAGEJEU] Ligne 33 : Affichage portée du jeu de note");

        std::cout << "[PageJeu] Widget Portee ajoute au layout." << std::endl;
    }
    else
    {
        // Configuration pour les jeux d'accords
        noteLabel = new QLabel("Accord en attente...", this);
        noteLabel->setAlignment(Qt::AlignCenter);
        noteLabel->setStyleSheet("font-size: 32px; font-weight: bold; font-family: Arial Black;");
        mainLayout->addWidget(noteLabel);
        std::cout << "[PageJeu] Configuration pour Jeu d'accord." << std::endl;
        Logger::log("[PAGEJEU] Ligne 45 : Affichage des accords des jeux d'accords");
    }

    // Boutons Pause et Quitter
    pauseButton = new QPushButton("Pause", this);
    pauseButton->setFixedHeight(60);                                                             // Augmenter la hauteur
    pauseButton->setStyleSheet("font-size: 30px; font-weight: bold; font-family: Arial Black;"); // Police moins fine
    quitterButton = new QPushButton("Quitter le jeu", this);
    quitterButton->setFixedHeight(60);                                                             // Augmenter la hauteur
    quitterButton->setStyleSheet("font-size: 30px; font-weight: bold; font-family: Arial Black;"); // Police moins fine
    mainLayout->addWidget(pauseButton);
    mainLayout->addWidget(quitterButton);

    // Connexions des signaux pour les boutons
    connect(pauseButton, &QPushButton::clicked, this, &PageJeu::togglePause);
    connect(quitterButton, &QPushButton::clicked, this, &PageJeu::quitterJeu);

    std::cout << "[IHM] Tentative de connexion au serveur MDJ..." << std::endl;

    // Initialisation de la connexion avec le serveur
    if (!socketManager.initialiserClient("127.0.0.1", 8080))
    {
        std::cerr << "[IHM] Erreur : Connexion au MDJ impossible." << std::endl;
        Logger::log("[PAGEJEU] Ligne 45 : Connexion au serveur (MDJ) impossible", true);

        if (jeuType == "Jeu d'accord SR" || jeuType == "Jeu d'accord AR")
        {
            Logger::log("[PAGEJEU] Ligne 72 : Connexion au serveur (MDJ) impossible", true);

            noteLabel->setText("Connexion echouee. Veuillez reessayer.");
        }
        return;
    }

    // Envoi des parametres du jeu au serveur
    QJsonObject params = {
        {"jeu", jeu},
        {"gamme", gamme},
        {"mode", mode},
        {"jouerSon", jouerSon}};
    socketManager.envoyerMessage(params);
    QString paramsQString = QString(QJsonDocument(params).toJson(QJsonDocument::Compact));
    Logger::log("[PAGEJEU] Ligne 86 : envoi des paramètres au MDJ : " + paramsQString.toStdString());

    std::cout << "[IHM] Parametres envoyes au serveur : "
              << QString(QJsonDocument(params).toJson(QJsonDocument::Compact)).toStdString() << std::endl;

    // Connexion pour gerer les messages recus depuis le serveur
    connect(&socketManager, &SocketManager::messageRecu, this, &PageJeu::recevoirMessage);
    std::cout << "[IHM] Page de jeu initialisee avec succes." << std::endl;
    Logger::log("[PAGEJEU] Ligne 95 : Page de jeu initialisée avec succes");
}
// Gestion des messages recus depuis le serveur
void PageJeu::recevoirMessage(const QString &message)
{
    auto jsonData = socketManager.deserialiserJson(message);

    if (jsonData.empty())
    {
        std::cerr << "[PageJeu] Erreur : Message JSON invalide ou vide recu." << std::endl;
        Logger::log("[PAGEJEU] Ligne 105 : Message JSON invalide ou vide recu.", true);

        return;
    }

    // Mise a jour de l'affichage en fonction du type de jeu
    if (jeuType == "Jeu d'accord SR" || jeuType == "Jeu d'accord AR")
    {
        if (jsonData.find("nom_accord") != jsonData.end())
        {
            QString nomAccord = QString::fromStdString(jsonData["nom_accord"]);
            noteLabel->setText("Accord : " + nomAccord);
            std::cout << "[PageJeu] Accord recu : " << nomAccord.toStdString() << std::endl;
            std::ostringstream nomAccordOstring;
            nomAccordOstring << "[PAGEJEU] Ligne 118 : Accord affiché : " << nomAccord.toStdString();
            Logger::log(nomAccordOstring.str());
        }
    }
    else if (jsonData.find("note") != jsonData.end())
    {
        QString note = QString::fromStdString(jsonData["note"]);
        portee->setNoteToDisplay(note);
        std::cout << "[PageJeu] Note recue : " << note.toStdString() << std::endl;
        std::ostringstream nomNoteOstring;
        nomNoteOstring << "[PAGEJEU] Ligne 129 : Note affiché : " << note.toStdString();
        Logger::log(nomNoteOstring.str());
    }

    // Detection de la fin du jeu
    if (jsonData.find("type") != jsonData.end() && jsonData["type"] == "fin_du_jeu")
    {
        if (jsonData.find("score") != jsonData.end())
        {
            try
            {
                int score = std::stoi(jsonData["score"]);

                std::ostringstream scoreOstring;
                scoreOstring << "[PAGEJEU] Ligne 143 : Score affiché : " << score;

                Logger::log(scoreOstring.str());
                emit jeuTermine(score);
            }
            catch (const std::exception &)
            {
                std::cerr << "[PageJeu] Erreur : Score non valide." << std::endl;
                Logger::log("[PAGEJEU] Ligne 151 : Score non valide", true);
            }
        }
    }
}

// Envoi des resultats au serveur
void PageJeu::envoyerResultat(bool correct)
{
    QJsonObject resultatMessage = {
        {"type", "resultat"},
        {"status", correct ? "correct" : "incorrect"}};
    socketManager.envoyerMessage(resultatMessage);
    std::cout << "[IHM] Resultat envoye au serveur : "
              << (correct ? "correct" : "incorrect") << std::endl;
}

// Gestion de la pause
void PageJeu::togglePause()
{
    enPause = !enPause;
    pauseButton->setText(enPause ? "Reprendre" : "Pause");

    QJsonObject pauseMessage = {
        {"type", "pause"},
        {"status", enPause ? "paused" : "resumed"}};
    socketManager.envoyerMessage(pauseMessage);
    std::cout << "[IHM] Pause " << (enPause ? "activee" : "desactivee") << std::endl;
}

// Gestion du bouton Quitter
void PageJeu::quitterJeu()
{
    std::cout << "[IHM] Le joueur a quitte le jeu." << std::endl;
    Logger::log("[PAGEJEU] Ligne 185 : Le joueur a quitté le jeu");

    emit quitterJeuSignal(); // Emettre un signal pour informer d'autres composants
    close();                 // Fermer la fenetre
}
