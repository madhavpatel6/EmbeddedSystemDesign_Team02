#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include "picCode/communication/messages.h"
#include <QJsonArray>

#define ANALOG 0

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = 0);
    int  send(QByteArray words);
    void connectToHost(QString ip,int port);
    QTcpSocket* getClient();

    bool isConnected;

signals:
    void sendCommStatTargetLocator(QByteArray commStatTargetLocator);
    void serverIsConnectedSignal(bool connectedToServerBoolSignal);
    void sendMovement(char source, QString x, QString y, QString orientation, QString action, QString amount);
    void sendLineLocation(int);
    void initialRequest();
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void movementRequested();
    void lineLocationRequested();
    void sendForwardCommand(int distance);
    void sendBackCommand(int distance);
    void sendLeftCommand(int degrees);
    void sendRightCommand(int degrees);
    void sendInitialData(char mode);

private:
    void SendJSONRequestToSocket(QString request, char destination);
    void SendJSONResponseToSocket(QString response, char destination);
    void HandleResponse(QJsonObject obj);
    void HandleRequest(QJsonArray array);
    int numOfErrors;
    QTcpSocket *socket;

};

#endif // CLIENTSOCKET_H
