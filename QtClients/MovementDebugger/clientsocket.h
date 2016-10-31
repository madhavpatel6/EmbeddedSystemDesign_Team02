#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include "picCode/communication/messages.h"

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
    void sendLocation(char source, QString x, QString y);
    void sendOrientation(char source, QString orientation);
    void sendLineLocation(int);
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void positionRequested();
    void lineLocationRequested();
    void sendForwardCommand(int distance);
    void sendBackCommand(int distance);
    void sendLeftCommand(int degrees);
    void sendRightCommand(int degrees);

private:
    void SendJSONRequestToSocket(QString request, char destination);
    void HandleResponse(QJsonObject obj);
    int numOfErrors;
    QTcpSocket *socket;

};

#endif // CLIENTSOCKET_H
