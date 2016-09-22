#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = 0);

    void connectToHost(QString ip,int port);
    int  send(QByteArray words);

signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();

    // make the server fully ascynchronous
    // by doing time consuming task
    void TaskResult(int Number);

private:
    QTcpSocket *socket;

};

#endif // CLIENTSOCKET_H
