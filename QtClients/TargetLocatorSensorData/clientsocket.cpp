// clientsocket.cpp

#include "clientsocket.h"
#include "mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
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

void ClientSocket::sendRequest(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[\"";
    QString request_end = "\"]}";
    SendJSONRequestToSocket(request_begin + "TargetLocatorSensorData" + request_end, TARGETLOCATOR);
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
                if(json.contains(QStringLiteral("TargetLocatorSensorData"))) {
                    HandleResponse(json);
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

void ClientSocket::SendJSONRequestToSocket(QString request, char destination) {
    if(isConnected == true){
        char message[512];
        int len = CreateMessage(message, request.toLatin1().data(), destination, 0);

        QByteArray txMessage;
        txMessage.setRawData(message, len);


        int bytesSent = send(txMessage);
        emit sentCommStatSignal();
        socket->flush();
    }
    else{
        qDebug() << "Client is not connected to the server. \n";
    }
}

void ClientSocket::HandleResponse(QJsonObject obj) {
    if(obj.contains(QStringLiteral("TargetLocatorSensorData"))) {
        QJsonObject ir = obj["TargetLocatorSensorData"].toObject()["IR"].toObject();

        emit sendUpdate(ir["farLeftFB"].toString(), ir["leftFB"].toString(), ir["middleFB"].toString(), ir["rightFB"].toString(), ir["farRightFB"].toString(), ir["leftFT"].toString(), ir["rightFT"].toString());
    }

}
