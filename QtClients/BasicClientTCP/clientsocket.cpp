// clientsocket.cpp

#include "clientsocket.h"

ClientSocket::ClientSocket(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket();

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void ClientSocket::connectToHost(QString ip,int port){
    socket->connectToHost(ip,port);
}

// asynchronous - runs separately from the thread we created
void ClientSocket::connected()
{
    qDebug() << "Client connected event";
}

// asynchronous
void ClientSocket::disconnected()
{
    qDebug() << "Client disconnected";
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

// After a task performed a time consuming task,
// we grab the result here, and send it to client
void ClientSocket::TaskResult(int Number)
{
    QByteArray Buffer;
    Buffer.append("\r\nTask result = ");
    Buffer.append(QString::number(Number));

    socket->write(Buffer);
}
