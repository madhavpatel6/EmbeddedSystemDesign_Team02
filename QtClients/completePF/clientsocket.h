#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "picCode/communication/messages.h"


class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = 0);
    int  send(QByteArray words);
    void connectToHost(QString ip,int port);
    void disconnectFromHost();
    void sendProximity(int proxVal);
    void sendMovement(bool movementBool);
    void sendOutsideArena(bool arenaBool);
    void targetAcquiredRequested(bool targetAcquiredReqBool);
    ~ClientSocket();
    QTcpSocket* getClient();

    bool isConnected;

signals:
    void sendCommStatTargetLocator(QByteArray commStatTargetLocator);
    void serverIsConnectedSignal(bool connectedToServerBoolSignal);
    void sendAlignmentSignal(char source, QString alignmentData0,
                             QString alignmentData1, QString alignmentData2,
                             QString alignmentData3, QString alignmentData4,
                             QString distanceFromTarget, QString angleToTarget,
                             QString alignmentBool_0, QString alignmentBool_1,
                             QString alignmentBool_2, QString alignmentBool_3,
                             QString alignmentBool_4, QString alignmentGrabberBool,
                             QString alignmentInfo);
    void sendAcquiredSignal(QString targetAcuiredBool);
    void sendCurRespSignal(QString curResp);
    void sendCurReqSignal(QString curReq);
    void sendtgCurRespSignal(QString tgcurResp);
    void sendtgCurReqSignal(QString tgcurReq);

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void alignmentRequestedSlot();

private:
    void SendJSONRequestToSocket(QString request, char destination);
    void HandleAlignmentResponse(QJsonObject obj, char source);
    void HandleAcquiredResponse(QJsonObject obj, char source);
    void HandleTGRequest(QJsonObject obj, char source);
    QTcpSocket *socket;

};

#endif // CLIENTSOCKET_H
