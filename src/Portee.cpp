#include "../include/Portee.h"
#include <QPainter>
#include <QMap>
#include <iostream>
#include "Logger.h"

// Constructeur par defaut
Portee::Portee(QWidget *parent) : QWidget(parent), noteToDisplay("")
{
    Logger::log("[Portee] ligne 9 : Initialisation de la portee");
}

// Methode pour definir la note a afficher
void Portee::setNoteToDisplay(const QString &note)
{
    noteToDisplay = note;
    Logger::log("[Portee] ligne 16 : Note a afficher : " + note.toStdString());
    update(); // Demande un rafraichissement de l'affichage
}

// Methode pour dessiner la portee et la note
void Portee::paintEvent(QPaintEvent *event)
{
    Logger::log("[Portee] ligne 23 : Demarrage du dessin de la portee");

    QPainter painter(this);

    // Configurer l'epaisseur des lignes de la portee
    QPen pen(Qt::black, 4); // Lignes plus epaisses
    painter.setPen(pen);

    // Dimensions de base
    int startX = 50;      // Position de depart en X
    int startY = 100;     // Position de depart en Y
    int lineSpacing = 30; // Espacement entre les lignes
    int lineCount = 5;    // Nombre de lignes de la portee
    int noteRadius = 15;  // Rayon des notes (plus grand)

    // Dessiner la portee en cle de sol
    Logger::log("[Portee] ligne 39 : Dessin de la portee en cle de sol");
    for (int i = 0; i < lineCount; ++i)
    {
        int y = startY + i * lineSpacing;
        painter.drawLine(startX, y, startX + 500, y); // Ligne plus longue
    }

    // Ajouter le symbole de la cle de sol
    painter.setFont(QFont("Bravura", 48));                              // Changer pour une police musicale
    painter.drawText(startX - 50, startY + lineSpacing * 2,"\U0001D11E"); // Symbole cle de sol

    // Ajuster la position pour la portee en cle de fa
    Logger::log("[Portee] ligne 51 : Dessin de la portee en cle de fa");
    int startYFa = startY + 200; // Distance augmentee entre les deux portees 

    // Dessiner la portee en cle de fa
    for (int i = 0; i < lineCount; ++i)
    {
        int y = startYFa + i * lineSpacing;
        painter.drawLine(startX, y, startX + 500, y);
    }

    // Ajouter le symbole de la cle de fa
    painter.setFont(QFont("Bravura", 48, QFont::Bold));                                 // Assurer que la police est correcte
    painter.drawText(startX - 50, startYFa + lineSpacing * 2,"\U0001D122"); // Symbole cle de fa
    // Positions des notes
    QMap<QString, int> notePositions = {
        // Cle de sol
        {"C4", startY + lineSpacing * 5},
        {"D4", startY + lineSpacing * 4.5},
        {"E4", startY + lineSpacing * 4},
        {"F4", startY + lineSpacing * 3.5},
        {"G4", startY + lineSpacing * 3},
        {"A4", startY + lineSpacing * 2.5},
        {"B4", startY + lineSpacing * 2},
        {"C5", startY + lineSpacing * 1.5},
        {"D5", startY + lineSpacing * 1},
        {"E5", startY + lineSpacing * 0.5},
        {"F5", startY},
        {"G5", startY - lineSpacing * 0.5},
        {"A5", startY - lineSpacing},
        {"B5", startY - lineSpacing * 1.5},
        {"C6", startY - lineSpacing * 2},

        // Cle de fa
        {"C2", startYFa + lineSpacing * 6},
        {"D2", startYFa + lineSpacing * 5.5},
        {"E2", startYFa + lineSpacing * 5},
        {"F2", startYFa + lineSpacing * 4.5},
        {"G2", startYFa + lineSpacing * 4},
        {"A2", startYFa + lineSpacing * 3.5},
        {"B2", startYFa + lineSpacing * 3},
        {"C3", startYFa + lineSpacing * 2.5},
        {"D3", startYFa + lineSpacing * 2},
        {"E3", startYFa + lineSpacing * 1.5},
        {"F3", startYFa + lineSpacing * 1},
        {"G3", startYFa + lineSpacing * 0.5},
        {"A3", startYFa},
        {"B3", startYFa - lineSpacing * 0.5}};

    // Dessiner la note si une note est definie
    if (!noteToDisplay.isEmpty())
    {
        Logger::log("[Portee] ligne 102 : Affichage de la note : " + noteToDisplay.toStdString());
        QString baseNote = noteToDisplay;
        QString modifier;

        // Gerer les dieses et bemols
        if (noteToDisplay.contains("#"))
        {
            baseNote = noteToDisplay.split("#")[0] + noteToDisplay.right(1); // Note + octave
            modifier = "#";
        }
        else if (noteToDisplay.contains("b"))
        {
            baseNote = noteToDisplay.split("b")[0] + noteToDisplay.right(1); // Note + octave
            modifier = "b";
        }

        if (notePositions.contains(baseNote))
        {
            int noteX = startX + 250; // Position horizontale de la note (centree)
            int noteY = notePositions[baseNote];

            // Dessiner le symbole du diese ou bemol
            if (!modifier.isEmpty())
            {
                painter.setFont(QFont("Arial Black", 36)); // Police pour les symboles
                painter.drawText(noteX - 55, noteY + 10, modifier);
            }

            // Dessiner la note
            painter.setBrush(Qt::black);
            painter.drawEllipse(QPoint(noteX, noteY), noteRadius, noteRadius);

            Logger::log("[Portee] ligne 134 : Note dessinee a la position (" + std::to_string(noteX) + ", " + std::to_string(noteY) + ")");

            // Gestion des lignes supplementaires pour notes extremes
            QPen linePen(Qt::black, 5); // Ligne supplementaire plus epaisse
            painter.setPen(linePen);
            if (baseNote == "E2" || baseNote == "C4" || baseNote == "A5")
            {
                painter.drawLine(noteX - 25, noteY, noteX + 25, noteY); // Ligne plus large
            }
            if (baseNote == "C2" || baseNote == "C6")
            {
                int offset = lineSpacing;
                painter.drawLine(noteX - 25, noteY, noteX + 25, noteY);
                int middleLineY = (baseNote == "C6") ? noteY + offset : noteY - offset;
                painter.drawLine(noteX - 25, middleLineY, noteX + 25, middleLineY);
            }
            if (baseNote == "D2" || baseNote == "B5")
            {
                int nearLineY = (baseNote == "D2") ? noteY - lineSpacing / 2 : noteY + lineSpacing / 2;
                painter.drawLine(noteX - 25, nearLineY, noteX + 25, nearLineY);
            }
        }
        else
        {
            Logger::log("[Portee] ligne 158 : Erreur - Note inconnue : " + noteToDisplay.toStdString());
        }
    }
}