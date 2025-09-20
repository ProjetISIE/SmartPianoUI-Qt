#ifndef PAGE_RESULTATS_H
#define PAGE_RESULTATS_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

/**
 * @class PageResultats
 * @brief Classe representant la page des resultats apres un jeu.
 *
 * Cette classe affiche le score final obtenu par le joueur
 * et permet de revenir a la page d'accueil.
 */
class PageResultats : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe PageResultats.
     * @param score Le score final obtenu par le joueur.
     * @param parent Pointeur vers le widget parent (par defaut nullptr).
     */
    explicit PageResultats(int score, QWidget *parent = nullptr);

signals:
    /**
     * @brief Signal emis lorsque l'utilisateur souhaite retourner a l'accueil.
     */
    void retourAccueil();

private slots:
    /**
     * @brief Slot pour gerer le clic sur le bouton de retour a l'accueil.
     */
    void onRetourAccueilClicked();

private:
    QLabel *scoreLabel;          ///< Label affichant le score final.
    QPushButton *accueilButton;  ///< Bouton pour retourner a l'accueil.
};

#endif // PAGE_RESULTATS_H
