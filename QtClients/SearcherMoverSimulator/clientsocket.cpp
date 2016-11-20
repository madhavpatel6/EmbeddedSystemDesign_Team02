// clientsocket.cpp

#include "clientsocket.h"
#include "mainwindow.h"
#include <QJsonArray>
ClientSocket::ClientSocket(QObject *parent) :
    QObject(parent)
{
    numOfErrors = 0;
    socket = new QTcpSocket();

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    isConnected = false;
}


void ClientSocket::connectToHost(QString ip,int port){
    socket->disconnectFromHost();
    socket->connectToHost(ip,port);
}

QTcpSocket* ClientSocket::getClient(){
    return socket;
}

void ClientSocket::sendPositionUpdate(QString x, QString y, QString angle){
    QString response = "{\"type\":\"Response\",\"R1_Movement\":[\"" + x + "\",\"" + y + "\",\"" + angle + "\",\"F\",\"0\"]}";
    SendJSONRequestToSocket(response, TARGETLOCATOR);
}

void ClientSocket::sendProximityRequest() {
    QString request = "{\"type\":\"Request\",\"items\":[\"ProximityInformation\"]}";
    SendJSONRequestToSocket(request, TARGETLOCATOR);
}

void ClientSocket::connected()
{
    qDebug() << "Client connected event";
    isConnected = true;
    emit serverIsConnectedSignal(true);
}


void ClientSocket::disconnected()
{
    qDebug() << "Client disconnected";
    isConnected = false;
    emit serverIsConnectedSignal(false);
}

int  ClientSocket::send(QByteArray words){
    return socket->write(words);
}

void ClientSocket::readyRead()
{
    //qDebug() << "ClientSocket::readyRead()";
    QByteArray array = socket->readAll();
    char buffer[MAXMESSAGESIZE];
    char source, messageCount;
    bool isError;
    for(int i = 0; i < array.size(); i++) {
        bool isCompleted = ParseMessage(array[i], buffer, &source, &messageCount, &isError);
        if(isCompleted) {
            QJsonDocument doc(QJsonDocument::fromJson(buffer));
            QJsonObject json = doc.object();
            QString type = json["type"].toString();
            if(type == QStringLiteral("Response")) {
                if(json.contains(QStringLiteral("ProximityInformation"))) {
                    HandleCommStatsResponse(json);
                }
            }
            else if(type == QStringLiteral("Request")){
                //qDebug() << "Request: " << buffer;
            }
        }
        else if(isError){
            numOfErrors++;
            emit updateError(numOfErrors);
        }
    }
}

void ClientSocket::SendJSONRequestToSocket(QString request, char destionation) {
    if(isConnected == true){
        //qDebug() << "Sending Request: " << request;
        char message[512];
        int len = CreateMessage(message, request.toLatin1().data(), destionation, 0);

        QByteArray txMessage;
        txMessage.setRawData(message, len);

        // qDebug() << txMessage << endl;

        int bytesSent = send(txMessage);
        //qDebug() << "bytesSent:" << bytesSent;
        emit sentCommStatSignal();
        //qDebug() << "Flushing Socket";
        socket->flush();
        //qDebug() << "Flushed Socket";
    }
    else{
        qDebug() << "Client is not connected to the server. \n";
    }
}

void ClientSocket::HandleCommStatsResponse(QJsonObject obj) {
    QJsonArray array = obj["ProximityInformation"].toArray();
    updateProximity(array[0].toString(),array[1].toString(),array[2].toString());
}
