#ifndef PORTEE_H
#define PORTEE_H

#include <QWidget>
#include <QString>

/**
 * @class Portee
 * @brief Classe representant une portee musicale avec la possibilite d'afficher une note specifique.
 *
 * Cette classe est utilisee pour dessiner une portee musicale et afficher une note donnee.
 */
class Portee : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe Portee.
     * @param parent Pointeur vers le widget parent (par defaut nullptr).
     */
    explicit Portee(QWidget *parent = nullptr);

    /**
     * @brief Definit la note a afficher sur la portee.
     * @param note Une chaine representant la note (par exemple, "C4", "D#5").
     */
    void setNoteToDisplay(const QString &note);

    /**
     * @brief Acceder a noteToDisplay depuis la classe de test unitaire.
     */
    QString getNoteToDisplay() const { return noteToDisplay; }

protected:
    /**
     * @brief Methode de dessin surchargee pour afficher la portee et la note.
     * @param event Evenement de peinture.
     */
    void paintEvent(QPaintEvent *event) override;

private:
    QString noteToDisplay; ///< Note a afficher sur la portee.
};

#endif // PORTEE_H
