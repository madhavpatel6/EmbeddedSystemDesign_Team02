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
    void updateError(int val);
    void sendCommStatTargetLocator(QByteArray commStatTargetLocator);
    void serverIsConnectedSignal(bool connectedToServerBoolSignal);
    void sentCommStatSignal();
    void sendCommStat(char source, QString goodMsg, QString commError, QString reqRecv, QString resRecv, QString reqSent, QString resSent);
    void updateProximity(QString farLeft, QString middle, QString farRight);
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void sendPositionUpdate(QString x, QString y, QString angle);
    void sendProximityRequest();
private:
    void SendJSONRequestToSocket(QString request, char destionation);
    void HandleCommStatsResponse(QJsonObject obj);
    int numOfErrors;
    QTcpSocket *socket;

};

#endif // CLIENTSOCKET_H
