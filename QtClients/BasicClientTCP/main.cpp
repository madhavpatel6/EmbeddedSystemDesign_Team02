#include <QCoreApplication>
#include <QDebug>
#include <QTcpSocket>

#include "clientsocket.h"
#include "initialization.h"
#include "picCode/communication/messages.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QJsonObject config = initialization::getConfig();

    QString serverIp = config.value("serverIp").toString();
    int serverPort = config.value("serverPort").toInt();

    ClientSocket *client = new ClientSocket();

    client->connectToHost(serverIp,serverPort);

//    qDebug() << "waiting for server\n" ;
//    while( client->isConnected == false){
//        QThread::msleep(100);
//    }
//    qDebug() << "done waiting for sever\n";

//    for(int i = 0; i < 20; i++){

//            QString request = "{\"type\":\"Request\",\"items\":[\"CommStatsTargetLocator\"]}";

//            char message[512];
//            int len = CreateMessage(message, request.toLatin1().data(), TARGETLOCATOR, i);

//            QByteArray txMessage;
//            txMessage.setRawData(message, len);

//            int bytesSent = client->send(txMessage);
//            qDebug() << "bytesSent:" << bytesSent << "\n";

//            QThread::msleep(100);
//        }




    return a.exec();
}
