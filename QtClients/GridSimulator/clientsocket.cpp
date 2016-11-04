// clientsocket.cpp

#include "clientsocket.h"
#include "mainwindow.h"
#include <QJsonDocument>
#include <QJsonArray>
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
    SendJSONRequestToSocket(request_begin + "OccupancyGrid" + request_end, TARGETLOCATOR);
    SendJSONRequestToSocket(request_begin + "LocationInformation" + request_end, TARGETLOCATOR);
//    SendJSONRequestToSocket(request_begin + "TimerTickCount" + request_end, TARGETLOCATOR);
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

#include <QByteArray>

int chartoint(char c) {
    return c - '0';
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
             char* s = strstr(buffer, "data");      // search for string "hassasin" in buff
             if (s != NULL)                     // if successful then s now points at "hassasin"
             {
                s += 7;
                char* r = strstr(buffer, "row");
                int index = 0;
                int row = 0;
                r += 6;
                char* itr = r;
                while(*itr != '\"') {
                    itr++;
                    index++;
                }
                itr = r;
                for(int i = 0; i < index; i++) {
                    row += chartoint(*itr)*pow(10,(index-i - 1));
                    itr++;
                }
                itr = s;
                QVector<char> vec;
                for(int i = 0; i < 50; i++) {
                    vec.push_back(*itr);
                    itr++;
                }

                HandleResponse(row, vec);
             }                                  // index of "hassasin" in buff can be found by pointer subtraction
             else
             {
                QJsonDocument doc(QJsonDocument::fromJson(buffer));
                QJsonObject json = doc.object();
                QString type = json["type"].toString();
                if(type == QStringLiteral("Response")) {
                    if(json.contains("LocationInformation")) {
                        QJsonArray arry = json["LocationInformation"].toArray();
                        QString a0 = arry[0].toString();
                        QString a1 = arry[1].toString();
                        QString a2 = arry[2].toString();
                        qDebug() << a0 << " " << a1 << " " <<a2;
                        emit updateRoverPosition(a0.toFloat(), a1.toFloat(), a2.toFloat());
                    }
                }
             }
//            QJsonDocument doc(QJsonDocument::fromJson(buffer));
//            QJsonObject json = doc.object();
//            QString type = json["type"].toString();
//            if(type == QStringLiteral("Response")) {
//                if(json.contains("OccupancyGrid")) {

//                    QJsonObject obj = json["OccupancyGrid"].toObject();

//                    HandleResponse(obj["row"].toString().toInt(), obj["data"].toString());
//                }
//            }
//            else if(type == QStringLiteral("Request")){

                //qDebug() << "Request: " << buffer;
//            }
        }
        else if(isError){
            numOfErrors++;
            emit updateError(numOfErrors);
        }
    }
}

void ClientSocket::SendJSONRequestToSocket(QString request, char destination) {
    if(isConnected == true){
        qDebug() << request;
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

void ClientSocket::HandleResponse(int row, QVector<char> vec) {
    emit updateGrid(row, vec);
}
