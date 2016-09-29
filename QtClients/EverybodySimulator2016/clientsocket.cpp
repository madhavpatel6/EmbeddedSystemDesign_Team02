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
    qDebug() << "Client connected event";
    isConnected = true;

//    for(int i = 0; i != -1; i++){

//            QString request = "{\"type\":\"Request\",\"items\":[\"CommStatsTargetLocator\"]}";

//            char message[512];
//            int len = CreateMessage(message, request.toLatin1().data(), TARGETLOCATOR, i);

//            QByteArray txMessage;
//            txMessage.setRawData(message, len);

//            // qDebug() << txMessage << endl;

//            int bytesSent = send(txMessage);
//            qDebug() << "bytesSent:" << bytesSent << "\n";

//            socket->flush();

//            QThread::msleep(200);
//        }
}

// asynchronous
void ClientSocket::disconnected()
{
    qDebug() << "Client disconnected";
    isConnected = false;
}

int  ClientSocket::send(QByteArray words){
    return socket->write(words);
}

// Our main thread of execution
// This happening via main thread
// Our code running in our current thread not in another QThread
// That's why we're getting the thread from the pool

void ClientSocket::readyRead()
{
    qDebug() << "ClientSocket::readyRead()";
    qDebug() << socket->readAll();

//    // Time consumer
//    MyTask *mytask = new MyTask();
//    mytask->setAutoDelete(true);

//    // using queued connection
//    connect(mytask, SIGNAL(Result(int)), this, SLOT(TaskResult(int)), Qt::QueuedConnection);

//    qDebug() << "Starting a new task using a thread from the QThreadPool";

//    // QThreadPool::globalInstance() returns global QThreadPool instance
//    QThreadPool::globalInstance()->start(mytask);

}
