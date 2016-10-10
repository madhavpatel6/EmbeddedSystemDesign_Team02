// clientsocket.cpp

#include "clientsocket.h"
#include "mainwindow.h"

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

void ClientSocket::positionRequested(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"R1_Est_Location\",\"R1_Est_Orientation\"" + request_end, SEARCHERMOVER);
}

void ClientSocket::sendForwardCommand(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Forward\"" + request_end, SEARCHERMOVER);
}

void ClientSocket::sendBackCommand(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Back\"" + request_end, SEARCHERMOVER);
}

void ClientSocket::sendLeftCommand(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Left\"" + request_end, SEARCHERMOVER);
}

void ClientSocket::sendRightCommand(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Right\"" + request_end, SEARCHERMOVER);
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
                if(json.contains(QStringLiteral("R1_Est_Location")) ||
                        json.contains(QStringLiteral("R1_Est_Orientation"))) {
                    HandleResponse(json);
                }
            }
            else if(type == QStringLiteral("Request")){
                //qDebug() << "Request: " << buffer;
            }
        }
        else if(isError){
            numOfErrors++;
        }
    }
}

void ClientSocket::SendJSONRequestToSocket(QString request, char destination) {
    if(isConnected == true){
        //qDebug() << "Sending Request: " << request;
        char message[512];
        int len = CreateMessage(message, request.toLatin1().data(), destination, 0);

        QByteArray txMessage;
        txMessage.setRawData(message, len);

        // qDebug() << txMessage << endl;

        int bytesSent = send(txMessage);
        //qDebug() << "bytesSent:" << bytesSent;
        emit sentPositionSignal();
        //qDebug() << "Flushing Socket";
        socket->flush();
        //qDebug() << "Flushed Socket";
    }
    else{
        qDebug() << "Client is not connected to the server. \n";
    }
}

void ClientSocket::HandleResponse(QJsonObject obj) {
    QJsonObject response;
    if(obj.contains(QStringLiteral("R1_Est_Location"))) {
        response = obj["R1_Est_Location"].toObject();
        emit sendLocation(SEARCHERMOVER, response["x"].toString(), response["y"].toString());
    }
    if(obj.contains(QStringLiteral("R1_Est_Orientation"))) {
        emit sendOrientation(SEARCHERMOVER, obj["R1_Est_Orientation"].toString());
        qDebug() << obj["R1_Est_Orientation"];
    }
}
