// clientsocket.cpp

#include "clientsocket.h"

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
    socket->connectToHost(ip,port);
}
void ClientSocket::disconnectFromHost(){
    socket->disconnectFromHost();
}

QTcpSocket* ClientSocket::getClient(){
    return socket;
}

// asynchronous - runs separately from the thread we created
void ClientSocket::connected()
{
    qDebug() << "Client connected to server";
    isConnected = true;
}

// asynchronous
void ClientSocket::disconnected()
{
    qDebug() << "Client disconnected from server";
    isConnected = false;
}

int  ClientSocket::send(QByteArray words){
    return socket->write(words);
}

void ClientSocket::readyRead()
{
    qDebug() << "ClientSocket::readyRead()";
    // qDebug() << socket->readAll();
    emit dataRead(socket->readAll());

}
