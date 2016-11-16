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
    void updateTime(QString time);
    void sendUpdate(QString leftFTSensor, QString rightFTSensor, QString leftFBSensor, QString middleFBSensor, QString rightFBSensor, QString leftUltra, QString middleUltra, QString rightUltra, QString leftUltraSide, QString rightUltraSide);
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void sendRequest();

private:
    void SendJSONRequestToSocket(QString request, char destination);
    void HandleResponse(QJsonObject obj);
    int numOfErrors;
    QTcpSocket *socket;

};

#endif // CLIENTSOCKET_H
