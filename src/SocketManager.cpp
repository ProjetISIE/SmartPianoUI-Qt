#include "../include/SocketManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include <iostream>
#include "Logger.h" 

// Constructeur
SocketManager::SocketManager(QObject *parent)
    : QObject(parent), socket(nullptr) {
    Logger::log("[SocketManager] ligne 11 : Initialisation de l'objet SocketManager");
}

// Destructeur
SocketManager::~SocketManager() {
    if (socket) {
        Logger::log("[SocketManager] ligne 17 : Deconnexion et nettoyage de la socket");
        socket->disconnectFromHost();
        socket->deleteLater();
    }
}

// Initialiser un client et se connecter au serveur
bool SocketManager::initialiserClient(const QString &adresse, int port) {
    Logger::log("[SocketManager] ligne 25 : Tentative de connexion au serveur (" + adresse.toStdString() + ":" + std::to_string(port) + ")");
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }

    socket = new QTcpSocket(this);
    socket->connectToHost(adresse, port);

    if (!socket->waitForConnected(5000)) {
        Logger::log("[SocketManager] ligne 36 : Erreur - Connexion au serveur echouee, etat du socket : " + std::to_string(socket->state()), true);
        return false;
    }

    connect(socket, &QTcpSocket::readyRead, this, &SocketManager::lireMessage);
    Logger::log("[SocketManager] ligne 41 : Connexion au serveur reussie");
    return true;
}

// Envoyer un message JSON au serveur
void SocketManager::envoyerMessage(const QJsonObject &message) {
    QJsonDocument doc(message);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    if (socket && socket->isOpen()) {
        qint64 bytesWritten = socket->write(data);
        if (bytesWritten > 0) {
            socket->flush();
            Logger::log("[SocketManager] ligne 54 : Message envoye au serveur : " + QString(data).toStdString());
        } else {
            Logger::log("[SocketManager] ligne 56 : Erreur - Echec de l'ecriture sur la socket", true);
        }
    } else {
        Logger::log("[SocketManager] ligne 59 : Erreur - Socket non ouverte ou invalide", true);
    }
}

// Lire un message recu (appele automatiquement via readyRead)
void SocketManager::lireMessage() {
    if (!socket) {
        Logger::log("[SocketManager] ligne 66 : Erreur - Socket inexistant lors de la reception", true);
        return;
    }

    QByteArray data = socket->readAll();
    Logger::log("[SocketManager] ligne 71 : Message recu : " + QString(data).toStdString());
    emit messageRecu(QString::fromUtf8(data));
}

// Lire un message immediatement avec timeout
QString SocketManager::recevoirMessage() {
    if (socket && socket->waitForReadyRead(3000)) {
        QByteArray data = socket->readAll();
        Logger::log("[SocketManager] ligne 79 : Message recu immediatement : " + QString(data).toStdString());
        return QString::fromUtf8(data);
    } else {
        Logger::log("[SocketManager] ligne 82 : Erreur - Aucun message recu ou socket non ouverte", true);
        return QString();
    }
}

// Serialiser un objet JSON
QJsonObject SocketManager::serialiserJson(const std::map<std::string, std::string> &data) {
    Logger::log("[SocketManager] ligne 89 : Serialisation d'un objet JSON");
    QJsonObject jsonObj;
    for (const auto &pair : data) {
        jsonObj[QString::fromStdString(pair.first)] = QString::fromStdString(pair.second);
    }
    return jsonObj;
}

// Deserialiser un message JSON
std::map<std::string, std::string> SocketManager::deserialiserJson(const QString &message) {
    Logger::log("[SocketManager] ligne 99 : Tentative de deserialisation d'un message JSON");
    std::map<std::string, std::string> data;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if (doc.isObject()) {
        QJsonObject jsonObj = doc.object();
        for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it) {
            data[it.key().toStdString()] = it.value().toString().toStdString();
        }
    } else {
        Logger::log("[SocketManager] ligne 109 : Erreur - Message JSON invalide : " + message.toStdString(), true);
    }

    return data;
}

// Gerer une nouvelle connexion pour un serveur
void SocketManager::gererNouvelleConnexion() {
    clientSocket = serveur->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &SocketManager::lireMessage);
    Logger::log("[SocketManager] ligne 119 : Nouvelle connexion client acceptee");
}

// Verifier si la socket est valide et ouverte
bool SocketManager::isSocketValide() const {
    Logger::log("[SocketManager] ligne 124 : Verification de l'etat de la socket");
    return socket && socket->isOpen();
}