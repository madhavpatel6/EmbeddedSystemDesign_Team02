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

void ClientSocket::lineLocationRequested(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"LineLocation\"" + request_end, SEARCHERMOVER);
}

void ClientSocket::sendForwardCommand(int distance){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Forward\":" + "\"" + QString::number(distance) + "\"" + request_end, SEARCHERMOVER);
    qDebug() << request_begin + "\"Forward\":" + "\"" + QString::number(distance) + "\"" + request_end;
}

void ClientSocket::sendBackCommand(int distance){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Back\":" + "\"" + QString::number(distance) + "\"" + request_end, SEARCHERMOVER);
    qDebug() << request_begin + "\"Back\":" + "\"" + QString::number(distance) + "\"" + request_end;
}

void ClientSocket::sendLeftCommand(int degrees){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Left\":" + "\"" + QString::number(degrees) + "\"" + request_end, SEARCHERMOVER);
    qDebug() << request_begin + "\"Left\":" + "\"" + QString::number(degrees) + "\"" + request_end;
}

void ClientSocket::sendRightCommand(int degrees){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Right\":" + "\"" + QString::number(degrees) + "\"" + request_end, SEARCHERMOVER);
    qDebug() << request_begin + "\"Right\":" + "\"" + QString::number(degrees) + "\"" + request_end;
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
                        json.contains(QStringLiteral("R1_Est_Orientation")) ||
                        json.contains(QStringLiteral("LineLocation"))) {
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

        send(txMessage);
        //qDebug() << "bytesSent:" << bytesSent;
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
    if(obj.contains(QStringLiteral("R1_Movement"))) {
        response = obj["R1_Movement"].toObject();
        emit sendMovement(SEARCHERMOVER, response["x"].toString(), response["y"].toString(),
                response["orientation"].toString(), response["action"].toString(),
                response["amount"].toString());
//        qDebug() << obj["R1_Movement"];
    }
    if(obj.contains(QStringLiteral("LineLocation"))) {
        int location = 0;
        response = obj["LineLocation"].toObject();
        for (int i = 0; i < 8; i++) {
            if (ANALOG) {
                location |= (response[QString::number(i)].toString().toFloat() < 512) << i;
            } else {
                location |= !(response[QString::number(i)].toString().toFloat()) << i;
            }

        }
        emit sendLineLocation(location);
//        qDebug() << obj["LineLocation"];
    }

}
