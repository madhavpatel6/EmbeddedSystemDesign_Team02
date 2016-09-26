#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include "picCode/communication/messages.h"
#include "mainwindow.h"

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
    void sentCommStatSignal(bool sentCommStatBoolSignal);
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void commStatRequested();
    //void connectToHost(QString ip,int port);

private:
    QTcpSocket *socket;

};

#endif // CLIENTSOCKET_H
