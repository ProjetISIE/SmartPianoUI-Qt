#ifndef PAGEACCUEIL_H
#define PAGEACCUEIL_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>

/**
 * @class PageAccueil
 * @brief Classe representant la page d'accueil de l'application.
 *
 * Cette page permet de selectionner le type de jeu, la gamme, le mode, et l'activation du son avant de lancer le jeu.
 */
class PageAccueil : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe PageAccueil.
     * @param parent Pointeur vers le widget parent (par defaut nullptr).
     */
    explicit PageAccueil(QWidget *parent = nullptr);

    //Fontion get pour les tests unitaires
    QComboBox* getJeuBox() const { return jeuBox; }
    QComboBox* getGammeBox() const { return gammeBox; }
    QComboBox* getModeBox() const { return modeBox; }
    QCheckBox* getSonCheckBox() const { return sonCheckBox; }

signals:
    /**
     * @brief Signal emis lorsque le joueur lance un jeu.
     * @param jeu Le type de jeu selectionne (par exemple, "Jeu de note").
     * @param gamme La gamme selectionnee (par exemple, "Do").
     * @param mode Le mode selectionne (par exemple, "Majeur").
     * @param jouerSon Indique si le son doit �tre activ� (true) ou d�sactiv� (false).
     */
    void lancerJeu(const QString &jeu, const QString &gamme, const QString &mode, bool jouerSon);

private slots:
    /**
     * @brief Slot appele lorsque le bouton "Lancer le jeu" est clique.
     */
    void onLancerJeu();

protected:
    QComboBox *jeuBox;       ///< ComboBox pour selectionner le type de jeu.
    QComboBox *gammeBox;     ///< ComboBox pour selectionner la gamme musicale.
    QComboBox *modeBox;      ///< ComboBox pour selectionner le mode (Majeur ou Mineur).
    QCheckBox *sonCheckBox;  ///< CheckBox pour activer ou desactiver le son.
    QPushButton *lancerButton; ///< Bouton pour lancer le jeu.
};

#endif // PAGEACCUEIL_H
