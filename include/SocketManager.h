#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QJsonObject>
#include <map>
#include <string>

/**
 * @class SocketManager
 * @brief Classe responsable de la gestion des communications via sockets TCP.
 *
 * Cette classe permet de g�rer un serveur TCP ou de se connecter � un serveur en tant que client.
 * Elle inclut des fonctionnalit�s pour envoyer et recevoir des messages JSON.
 */
class SocketManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructeur par d�faut de la classe SocketManager.
     * @param parent Objet parent optionnel.
     */
    explicit SocketManager(QObject *parent = nullptr);

    /**
     * @brief Destructeur de la classe SocketManager.
     */
    ~SocketManager();

    /**
     * @brief Initialise un serveur TCP sur un port sp�cifi�.
     * @param port Le port sur lequel d�marrer le serveur.
     * @return true si le serveur a �t� correctement initialis�, false sinon.
     */
    bool initialiserServeur(int port);

    /**
     * @brief Attend la connexion d'un client au serveur.
     *
     * Cette m�thode bloque jusqu'� ce qu'une connexion soit �tablie.
     */
    void attendreConnexion();

    /**
     * @brief Initialise un client et connecte � un serveur distant.
     * @param adresse Adresse IP ou hostname du serveur.
     * @param port Port du serveur.
     * @return true si la connexion a �t� �tablie avec succ�s, false sinon.
     */
    bool initialiserClient(const QString &adresse, int port);

    /**
     * @brief Envoie un message JSON via la socket connect�e.
     * @param message L'objet JSON � envoyer.
     */
    void envoyerMessage(const QJsonObject &message);

    /**
     * @brief V�rifie si la socket actuelle est valide et ouverte.
     * @return true si la socket est valide et connect�e, false sinon.
     */
    bool isSocketValide() const;

    /**
     * @brief Lit un message re�u via la socket.
     * @return Le message re�u sous forme de QString.
     */
    QString recevoirMessage();

    /**
     * @brief S�rialise un std::map en objet JSON.
     * @param data La map � s�rialiser.
     * @return Un QJsonObject repr�sentant la map.
     */
    QJsonObject serialiserJson(const std::map<std::string, std::string> &data);

    /**
     * @brief D�s�rialise un message JSON en std::map.
     * @param message Le message JSON � d�s�rialiser.
     * @return Une std::map contenant les donn�es extraites du JSON.
     */
    std::map<std::string, std::string> deserialiserJson(const QString &message);

signals:
    /**
     * @brief Signal �mis lorsqu'un message est re�u via la socket.
     * @param message Le message re�u sous forme de QString.
     */
    void messageRecu(const QString &message);

private:
    QTcpServer *serveur;         ///< Pointeur vers l'objet serveur TCP.
    QTcpSocket *clientSocket;    ///< Pointeur vers la socket du client connect�.
    QTcpSocket *socket;          ///< Pointeur vers la socket utilis�e (client ou serveur).

private slots:
    /**
     * @brief G�re les connexions entrantes pour le serveur.
     */
    void gererNouvelleConnexion();

    /**
     * @brief Lit les messages re�us via la socket.
     */
    void lireMessage();
};

#endif // SOCKET_MANAGER_H
