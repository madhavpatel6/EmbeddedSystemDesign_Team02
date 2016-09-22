#include <QCoreApplication>
#include <QDebug>
#include <QTcpSocket>

#include "clientsocket.h"
#include "initialization.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QJsonObject config = initialization::getConfig();

    QString serverIp = config.value("serverIp").toString();
    int serverPort = config.value("serverPort").toInt();

    ClientSocket *client = new ClientSocket();

    client->connectToHost(serverIp,serverPort);
    client->send(QString("Hello"));

    return a.exec();
}
