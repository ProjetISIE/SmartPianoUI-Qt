#include <QApplication>
#include "PageAccueil.h"
#include "PageJeu.h"
#include "PageResultats.h"
#include <iostream>
#include "SocketManager.h"
#include <QProcess>
#include "Logger.h"

// Objet SocketManager global pour gerer les communications reseau
SocketManager socketManager;

// Déclare un QProcess global pour gérer le processus MDJ
QProcess *mdjProcess = nullptr;

// Fonction pour démarrer le MDJ
void demarrerMDJ()
{
    if (!mdjProcess)
    {
        mdjProcess = new QProcess();
    }
    if (mdjProcess->state() == QProcess::NotRunning)
    {
        mdjProcess->setWorkingDirectory("../PianoTrainerMDJV1");     // Répertoire de travail = MDJ
        mdjProcess->start("../PianoTrainerMDJV1/PianoTrainerMDJV1"); // Chemin vers l'exécutable MDJ
        if (!mdjProcess->waitForStarted())
        {
            qWarning() << "[ERREUR] Impossible de démarrer le MDJ.";
        }
        else
        {
            qDebug() << "[INFO] MDJ démarré avec succès.";
        }
    }
}

// Fonction pour redemarrer le MDJ
void redemarrerMDJ()
{
    if (mdjProcess && mdjProcess->state() != QProcess::NotRunning)
    {
        mdjProcess->terminate(); // Arréte proprement le MDJ
        if (!mdjProcess->waitForFinished(3000))
        {
            mdjProcess->kill(); // Forcer l'arret si necessaire
        }
        qDebug() << "[INFO] MDJ arrété.";
    }
    demarrerMDJ(); // Relance le MDJ
}

// Fonction pour arréter le MDJ
void arreterMDJ()
{
    if (mdjProcess)
    {
        if (mdjProcess->state() != QProcess::NotRunning)
        {
            mdjProcess->terminate(); // Tente d'arréter proprement
            if (!mdjProcess->waitForFinished(3000))
            {
                mdjProcess->kill(); // Forcer l'arrét
            }
        }
        delete mdjProcess;
        mdjProcess = nullptr;
        qDebug() << "[INFO] MDJ arrete et supprime.";
    }
}

int main(int argc, char *argv[])
{
    // Initialisation de l'application Qt
    Logger::init("log_basique_IHM.txt", "log_erreurs_IHM.txt");
    QApplication app(argc, argv);
    std::cout << "[INFO] Application demarree." << std::endl;
    Logger::log("[MAIN] Ligne 78 : Application démarrée");

    // Demarrer le MDJ une premiere fois
    demarrerMDJ();
    Logger::log("[MAIN] Ligne 82 : Moteur de jeu démarrée");

    // Creation de la page d'accueil
    auto *accueil = new PageAccueil();
    accueil->show(); // Affiche la page d'accueil
    Logger::log("[MAIN] Ligne 87 : Page d'accueil affichée");

    // Connexion du signal "lancerJeu" pour passer à la page de jeu
    QObject::connect(accueil, &PageAccueil::lancerJeu, [=](const QString &jeu, const QString &gamme, const QString &mode, bool jouerSon)
                     {
        std::cout << "[INFO] Passage à la page de jeu. Jeu : "
                  << jeu.toStdString() << ", Gamme : "
                  << gamme.toStdString() << ", Mode : "
                  << mode.toStdString() << ", Option du son : "
                  << (jouerSon ? "true" : "false") << std::endl;

        std::ostringstream messageJeu;
        messageJeu << "[MAIN] Ligne 98 : [INFO] Passage a la page de jeu. Jeu : "
           << jeu.toStdString() << ", Gamme : "
           << gamme.toStdString() << ", Mode : "
           << mode.toStdString() << ", Option du son : "
           << (jouerSon ? "true" : "false");
        Logger::log(messageJeu.str());

        // Création de la page de jeu
        auto *jeuPage = new PageJeu(jeu, gamme, mode, jouerSon);
        jeuPage->show(); // Affiche la page de jeu
        accueil->hide(); // Cache la page d'accueil

        // Gestion des signaux pour quitter le jeu
        QObject::connect(jeuPage, &PageJeu::quitterJeu, [=]() {
            std::cout << "[INFO] Retour à la page d'accueil depuis la page de jeu." << std::endl;
            jeuPage->close(); // Ferme la page de jeu
            jeuPage->deleteLater(); // Nettoie la mémoire associée
            accueil->show(); // Affiche à nouveau la page d'accueil
        });

        // Gestion des signaux pour terminer le jeu
        QObject::connect(jeuPage, &PageJeu::jeuTermine, [=](int score) {
            std::cout << "[INFO] Passage à la page des résultats. Score : " << score << std::endl;


            // Création de la page des résultats
            auto *resultats = new PageResultats(score);
            resultats->show(); // Affiche la page des résultats
            Logger::log("[MAIN] Ligne 122 : Page de résultats affichée");


            // Fermeture et nettoyage de la page de jeu
            if (jeuPage) {
                std::cout << "[DEBUG] Fermeture de jeuPage." << std::endl;
                jeuPage->close();
                jeuPage->deleteLater();
                Logger::log("[MAIN] Ligne 135 : Fermeture de la page de jeu");

            }

            // Redémarrage du MDJ
            redemarrerMDJ();
            Logger::log("[MAIN] Ligne 141 : Redémarrage du MDJ");


            // Connexion du signal pour retourner à l'accueil depuis la page des résultats
            QObject::connect(resultats, &PageResultats::retourAccueil, [=]() {
                std::cout << "[INFO] Retour à la page d'accueil depuis les résultats." << std::endl;
                resultats->close(); // Ferme la page des résultats
                resultats->deleteLater(); // Nettoie la mémoire associée
                accueil->show(); // Affiche à nouveau la page d'accueil
                Logger::log("[MAIN] Ligne 150 : Retour à la page d'accueil");
            });

        }); });

    // Connecter la fermeture de l'application à l'arret du MDJ
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]()
                     {
        std::cout << "[INFO] Arret de l'application. Fermeture du MDJ." << std::endl;
        Logger::log("[MAIN] Ligne 159 : Fermeture du MDJ");
        arreterMDJ(); });

    // Execution de l'application Qt
    return app.exec();
}