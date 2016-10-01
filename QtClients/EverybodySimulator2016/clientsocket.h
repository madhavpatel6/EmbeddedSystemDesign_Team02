#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QDebug>
#include <QThreadPool>
#include "picCode/communication/messages.h"

#include <QTcpSocket>

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = 0);

    void connectToHost(QString ip,int port);
    void disconnectFromHost();
    int  send(QByteArray words);
    QTcpSocket* getClient();

    bool isConnected;


    QTcpSocket *socket;

signals:
    void dataRead(QByteArray in);

public slots:
    void connected();
    void disconnected();
    void readyRead();

private:

};

#endif // CLIENTSOCKET_H
