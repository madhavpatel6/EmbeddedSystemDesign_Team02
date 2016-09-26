#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    requestTimer = new QTimer();
    requestTimer->setInterval(REQUESTRATE_MS);
    ui->setupUi(this);
    tcpSocket = new ClientSocket();
    connect(requestTimer, SIGNAL(timeout()), tcpSocket, SLOT(commStatRequested()));
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(bool)), this, SLOT(HostConnectionEvent(bool)));
    connect(tcpSocket, SIGNAL(sentCommStatSignal()), this, SLOT(CommStatsRequestSent()));
    connect(tcpSocket, SIGNAL(sendCommStat(char,QString,QString,QString,QString,QString,QString)), this, SLOT(UpdateCommStats(char,QString,QString,QString,QString,QString,QString)));
    commStatRequestCounter = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete requestTimer;
    delete tcpSocket;
}

void MainWindow::on_connectToServerButton_clicked()
{
    tcpSocket->connectToHost(ui->serverIPAddressInput->text(), ui->serverPortInput->text().toInt());
}

void MainWindow::on_disconnectFromServerButton_clicked()
{

}
