#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include <QJsonDocument>
#include <QJsonObject>
#include "picCode/communication/messages.h"


class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = 0);
    int  send(QByteArray words);
    void connectToHost(QString ip,int port);
    void disconnectFromHost();
    ~ClientSocket();
    QTcpSocket* getClient();

    bool isConnected;

signals:
    void sendCommStatTargetLocator(QByteArray commStatTargetLocator);
    void serverIsConnectedSignal(bool connectedToServerBoolSignal);
    void sendAlignmentSignal(char source, QString alignmentData0, QString alignmentData1, QString alignmentData2, QString alignmentBool_0, QString alignmentBool_1, QString alignmentBool_2, QString alignmentGrabberBool);
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void alignmentRequestedSlot();

private:
    void SendJSONRequestToSocket(QString request, char destination);
    void HandleAlignmentResponse(QJsonObject obj, char source);
    QTcpSocket *socket;

};

#endif // CLIENTSOCKET_H
