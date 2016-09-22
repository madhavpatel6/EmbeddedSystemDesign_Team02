#include <QCoreApplication>
#include <QDebug>
#include <QTcpSocket>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpSocket socket;

    socket.connectToHost("172.31.113.140",2000);

    socket.write(QString("Hello Server!").toLatin1());
    return 0;
    // return a.exec();
}
