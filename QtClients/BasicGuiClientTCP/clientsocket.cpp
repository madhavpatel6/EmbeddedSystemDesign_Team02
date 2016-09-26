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

void ClientSocket::connectToHost(QString ip,int port){
    socket->disconnectFromHost();
    socket->connectToHost(ip,port);
}

QTcpSocket* ClientSocket::getClient(){
    return socket;
}

void ClientSocket::commStatRequested(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[\"";
    QString request_end = "\"]}";
    SendJSONRequestToSocket(request_begin + "CommStatsSearcherMover" + request_end, SEARCHERMOVER);
    SendJSONRequestToSocket(request_begin + "CommStatsTargetLocator" + request_end, TARGETLOCATOR);
    SendJSONRequestToSocket(request_begin + "CommStatsPathFinder" + request_end, PATHFINDER);
    SendJSONRequestToSocket(request_begin + "CommStatsTargetGrabber" + request_end, TARGETGRABBER);
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
                if(json.contains(QStringLiteral("CommStatsSearcherMover")) ||
                        json.contains(QStringLiteral("CommStatsTargetLocator")) ||
                        json.contains(QStringLiteral("CommStatsPathFinder")) ||
                        json.contains(QStringLiteral("CommStatsTargetGrabber"))) {
                    HandleCommStatsResponse(json);
                }
            }
            else if(type == QStringLiteral("Request")){
                //qDebug() << "Request: " << buffer;
            }
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
    QJsonObject comStats;
    if(obj.contains(QStringLiteral("CommStatsSearcherMover"))) {
        comStats = obj["CommStatsSearcherMover"].toObject();
        emit sendCommStat(SEARCHERMOVER, comStats["numGoodMessagesRecved"].toString(), comStats["numCommErrors"].toString(), comStats["numJSONRequestsRecved"].toString(), comStats["numJSONResponsesRecved"].toString(), comStats["numJSONRequestsSent"].toString(), comStats["numJSONResponsesSent"].toString());
        //qDebug() << "Response From SM";
    }
    else if(obj.contains(QStringLiteral("CommStatsTargetLocator"))) {
        comStats = obj["CommStatsTargetLocator"].toObject();
        emit sendCommStat(TARGETLOCATOR, comStats["numGoodMessagesRecved"].toString(), comStats["numCommErrors"].toString(), comStats["numJSONRequestsRecved"].toString(), comStats["numJSONResponsesRecved"].toString(), comStats["numJSONRequestsSent"].toString(), comStats["numJSONResponsesSent"].toString());
        //qDebug() << "Response From TL";
    }
    else if(obj.contains(QStringLiteral("CommStatsPathFinder"))) {
        comStats = obj["CommStatsPathFinder"].toObject();
        emit sendCommStat(PATHFINDER, comStats["numGoodMessagesRecved"].toString(), comStats["numCommErrors"].toString(), comStats["numJSONRequestsRecved"].toString(), comStats["numJSONResponsesRecved"].toString(), comStats["numJSONRequestsSent"].toString(), comStats["numJSONResponsesSent"].toString());
        //qDebug() << "Response From PF";
    }
    else if(obj.contains(QStringLiteral("CommStatsTargetGrabber"))) {
        comStats = obj["CommStatsTargetGrabber"].toObject();
        emit sendCommStat(TARGETGRABBER, comStats["numGoodMessagesRecved"].toString(), comStats["numCommErrors"].toString(), comStats["numJSONRequestsRecved"].toString(), comStats["numJSONResponsesRecved"].toString(), comStats["numJSONRequestsSent"].toString(), comStats["numJSONResponsesSent"].toString());
        //qDebug() << "Response From TG";
    }

}
