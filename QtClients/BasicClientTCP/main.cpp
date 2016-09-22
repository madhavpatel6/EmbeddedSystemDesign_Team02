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

    QString request = "{\"type\", \"Request\"}, \"list\": [\"commStats\"]}" ;

    char message[512];
    int len = CreateMessage(message, request.toLatin1().data(), 'l', 44);

    QByteArray txMessage;
    txMessage.setRawData(message, len);

    client->send(txMessage);

    return a.exec();
}
