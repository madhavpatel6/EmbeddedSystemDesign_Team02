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
    void sendResponse(QString x, QString y, QString theta) {
        QString response = "{\"type\":\"Response\",\"R1_Movement\":["
                                    "\""+x+"\","
                                    "\""+y+"\","
                                    "\""+theta+"\","
                                    "\"F\","
                                    "\"0\"]}";
        SendJSONRequestToSocket(response, TARGETLOCATOR);
    }
    void handleInterpret();
    void handleSendResponse() {
        QString request_begin = "{\"type\":\"Request\",\"items\":[\"";
        QString request_end = "\"]}";
        SendJSONRequestToSocket(request_begin + "StartResponding" + request_end, TARGETLOCATOR);
    }

signals:
    void updateError(int val);
    void sendCommStatTargetLocator(QByteArray commStatTargetLocator);
    void serverIsConnectedSignal(bool connectedToServerBoolSignal);
    void sentCommStatSignal();
    void updateTime(QString time);
    void sendUpdate(QString leftFTSensor, QString rightFTSensor, QString leftFBSensor, QString middleFBSensor, QString rightFBSensor, QString leftUltra, QString middleUltra, QString rightUltra, QString leftUltraSide, QString rightUltraSide);
    void updateGrid(int row, QVector<char> vec);
    void updateRoverPosition(float x, float y, float orientation);


public slots:
    void connected();
    void disconnected();
    void readyRead();
    void sendRequest(int i);

private:
    void SendJSONRequestToSocket(QString request, char destination);
    void HandleResponse(int row, QVector<char> vec);
    int numOfErrors;
    QTcpSocket *socket;

};

#endif // CLIENTSOCKET_H
