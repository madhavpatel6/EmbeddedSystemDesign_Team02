// clientsocket.cpp

#include "clientsocket.h"

ClientSocket::ClientSocket(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket();

    //connect(uiMainWindow, SIGNAL(on_commStatButton_clicked()), this, SLOT(commStatRequested()));
    //connect(this, SIGNAL(sendCommStatTargetLocator(QByteArray)), uiMainWindow, SLOT(on_targetLocatorTextEdit_textChanged(QByteArray)));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //connect(this, SIGNAL(serverIsConnectedSignal(bool)), uiMainWindow, SLOT(on_connectedToServerTextEdit_textChanged(bool)));
    //connect(this, SIGNAL(sentCommStatSignal(bool)), uiMainWindow, SLOT(on_sentCommStatTextEdit_textChanged(bool)));
    isConnected = false;
}

void ClientSocket::connectToHost(QString ip,int port){
    socket->connectToHost(ip,port);
    emit serverIsConnectedSignal(true);
}

QTcpSocket* ClientSocket::getClient(){
    return socket;
}

void ClientSocket::commStatRequested(){
    if(isConnected == true){
        QString request = "{\"type\":\"Request\",\"items\":[\"CommStatsTargetLocator\"]}";

        char message[512];
        int len = CreateMessage(message, request.toLatin1().data(), TARGETLOCATOR, 0);

        QByteArray txMessage;
        txMessage.setRawData(message, len);

        // qDebug() << txMessage << endl;

        int bytesSent = send(txMessage);
        qDebug() << "bytesSent:" << bytesSent << "\n";
        emit sentCommStatSignal(true);
        socket->flush();
    }
    else{
        qDebug() << "Client not yet connected. \n";
    }
}

// asynchronous - runs separately from the thread we created
void ClientSocket::connected()
{
    qDebug() << "Client connected event";
    isConnected = true;
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
    emit sendCommStatTargetLocator(socket->readAll());

//    // Time consumer
//    MyTask *mytask = new MyTask();
//    mytask->setAutoDelete(true);

//    // using queued connection
//    connect(mytask, SIGNAL(Result(int)), this, SLOT(TaskResult(int)), Qt::QueuedConnection);

//    qDebug() << "Starting a new task using a thread from the QThreadPool";

//    // QThreadPool::globalInstance() returns global QThreadPool instance
//    QThreadPool::globalInstance()->start(mytask);

}
