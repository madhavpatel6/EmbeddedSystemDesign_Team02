// clientsocket.cpp

#include "clientsocket.h"
#include "mainwindow.h"

ClientSocket::ClientSocket(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket();

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    isConnected = false;
}

ClientSocket::~ClientSocket()
{
    socket->disconnectFromHost();
    qDebug() << "Disconnected from host" << endl;
}

void ClientSocket::connectToHost(QString ip,int port)
{
    socket->connectToHost(ip,port);
}

QTcpSocket* ClientSocket::getClient()
{
    return socket;
}

void ClientSocket::alignmentRequestedSlot(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[\"";
    QString request_end = "\"]}";
    SendJSONRequestToSocket(request_begin + "SensorData" + request_end, TARGETGRABBER);
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
    qDebug() << "ClientSocket::readyRead()";
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
                if(json.contains(QStringLiteral("SensorData"))){
                    HandleAlignmentResponse(json);
                }
            }
            else if(type == QStringLiteral("Request")){
                //qDebug() << "Request: " << buffer;
            }
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
        int bytesSent = send(txMessage);
        socket->flush();
    }
    else{
        qDebug() << "Client is not connected to the server. \n";
    }
}

void ClientSocket::HandleAlignmentResponse(QJsonObject obj) {
    //qDebug() << "JSON : " << obj << endl;
    if(obj.contains(QStringLiteral("SensorData"))) {
        emit sendAlignmentSignal(TARGETGRABBER, obj["SensorData"].toString());
    }

}
