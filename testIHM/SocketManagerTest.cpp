#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/SocketManager.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include <QTest>

// Test de l'initialisation du client et de la connexion au serveur
TEST_CASE("SocketManager - Initialisation du client") {
    QTcpServer serveur;
    serveur.listen(QHostAddress::LocalHost, 8081);

    SocketManager client;
    CHECK(client.initialiserClient("127.0.0.1", 8081) == true);
    CHECK(client.isSocketValide() == true);
}

// Test de l'envoi d'un message JSON
TEST_CASE("SocketManager - Envoi de message JSON") {
    QTcpServer serveur;
    serveur.listen(QHostAddress::LocalHost, 8081);

    SocketManager client;
    CHECK(client.initialiserClient("127.0.0.1", 8081) == true);

    QJsonObject message = {{"type", "test"}, {"contenu", "message de test"}};
    client.envoyerMessage(message);

    CHECK(client.isSocketValide() == true);
}

// Test de la reception d'un message JSON
TEST_CASE("SocketManager - Reception de message JSON") {
    QTcpServer serveur;
    serveur.listen(QHostAddress::LocalHost, 8081);

    SocketManager client;
    CHECK(client.initialiserClient("127.0.0.1", 8081) == true);

    QTcpSocket socketServeur;
    socketServeur.connectToHost("127.0.0.1", 8081);
    CHECK(socketServeur.waitForConnected(3000) == true);

    QJsonObject message = {{"type", "reponse"}, {"contenu", "message recu"}};
    QJsonDocument doc(message);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    socketServeur.write(data);
    socketServeur.flush();
    socketServeur.waitForBytesWritten();

    QTest::qWait(500); // Attente pour la reception

}

// Test de la serialisation JSON
TEST_CASE("SocketManager - Serialisation JSON") {
    SocketManager manager;

    std::map<std::string, std::string> donnees = {
        {"nom", "test"},
        {"valeur", "42"}
    };

    QJsonObject jsonObj = manager.serialiserJson(donnees);
    CHECK(jsonObj["nom"].toString().toStdString() == "test");
    CHECK(jsonObj["valeur"].toString().toStdString() == "42");
}

// Test de la deserialisation JSON
TEST_CASE("SocketManager - Deserialisation JSON") {
    SocketManager manager;

    QString message = R"({"nom":"test","valeur":"42"})";
    std::map<std::string, std::string> donnees = manager.deserialiserJson(message);

    CHECK(donnees["nom"] == "test");
    CHECK(donnees["valeur"] == "42");
}

// Test de la validation de la socket
TEST_CASE("SocketManager - Verification de l'etat de la socket") {
    QTcpServer serveur;
    serveur.listen(QHostAddress::LocalHost, 8081);

    SocketManager client;
    CHECK(client.initialiserClient("127.0.0.1", 8081) == true);
    CHECK(client.isSocketValide() == true);
}
