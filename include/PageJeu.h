#ifndef PAGEJEU_H
#define PAGEJEU_H

#include "Portee.h"
#include "SocketManager.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QProcess>

/**
 * @class PageJeu
 * @brief Classe representant la page principale d'un jeu (jeu de notes ou jeu d'accords).
 *
 * Cette classe gere l'affichage du jeu, la reception des messages, et l'interaction avec le joueur.
 */
class PageJeu : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe PageJeu.
     * @param jeu Type de jeu (par exemple, "Jeu de note" ou "Jeu d'accord SR").
     * @param gamme Gamme choisie pour le jeu.
     * @param mode Mode choisi (par exemple, "Majeur" ou "Mineur").
     * @param jouerSon Son activé ou non.
     * @param parent Pointeur vers le widget parent (par defaut nullptr).
     */
    explicit PageJeu(const QString &jeu, const QString &gamme, const QString &mode, bool jouerSon, QWidget *parent = nullptr);

signals:
    /**
     * @brief Signal emis lorsque le jeu est termine.
     * @param score Le score obtenu par le joueur.
     */
    void jeuTermine(int score);

    /**
     * @brief Signal emis lorsque le joueur quitte le jeu.
     */
    void quitterJeuSignal();

public slots:
    /**
     * @brief Slot pour traiter les messages recus depuis le serveur.
     * @param message Message recu sous forme de QString.
     */
    void recevoirMessage(const QString &message);

    /**
     * @brief Slot pour envoyer un resultat de reponse (correct ou incorrect) au serveur.
     * @param correct Indique si la reponse etait correcte ou non.
     */
    void envoyerResultat(bool correct);

    /**
     * @brief Slot pour quitter le jeu.
     */
    void quitterJeu();

private slots:
    /**
     * @brief Active ou desactive le mode pause.
     */
    void togglePause();

private:
    QString jeuType;               ///< Type de jeu (par exemple, "Jeu de note" ou "Jeu d'accord SR").
    Portee *portee;                ///< Widget affichant la portee musicale.
    QLabel *noteLabel;             ///< Label affichant la note ou l'accord a jouer.
    QPushButton *pauseButton;      ///< Bouton pour activer/desactiver la pause.
    QPushButton *quitterButton;    ///< Bouton pour quitter le jeu.
    SocketManager socketManager;   ///< Gestionnaire de communication avec le serveur.
    QString noteAttendue;          ///< Note attendue par le jeu.
    bool enPause;                  ///< Indique si le jeu est en pause.
    int score;                     ///< Score actuel du joueur.
};

#endif // PAGEJEU_H
