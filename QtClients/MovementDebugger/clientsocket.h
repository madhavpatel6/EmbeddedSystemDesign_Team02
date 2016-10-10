#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include "picCode/communication/messages.h"


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
    void sentPositionSignal();
    void sendLocation(char source, QString x, QString y);
    void sendOrientation(char source, QString orientation);
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void positionRequested();
    void sendForwardCommand();
    void sendBackCommand();
    void sendLeftCommand();
    void sendRightCommand();

private:
    void SendJSONRequestToSocket(QString request, char destination);
    void HandleResponse(QJsonObject obj);
    int numOfErrors;
    QTcpSocket *socket;

};

#endif // CLIENTSOCKET_H
