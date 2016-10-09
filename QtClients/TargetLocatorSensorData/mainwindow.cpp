#include "mainwindow.h"
#include "ui_mainwindow.h"

#define REQUESTRATE_MS 500

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    requestTimer = new QTimer();
    requestTimer->setInterval(REQUESTRATE_MS);
    ui->setupUi(this);
    tcpSocket  = new ClientSocket();
    connect(requestTimer, SIGNAL(timeout()), tcpSocket, SLOT(sendRequest()));
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(bool)), this, SLOT(HostConnectionEvent(bool)));
    connect(tcpSocket, SIGNAL(sendUpdate(QString,QString,QString,QString,QString,QString,QString,QString)), this, SLOT(receiveUpdate(QString,QString,QString,QString,QString,QString,QString,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_requestButton_clicked()
{
    if(requestTimer->isActive()) {
        ui->requestButton->setText(QString("Request Sensor Data"));
        requestTimer->stop();
    }
    else {
        ui->requestButton->setText(QString("Stop Requesting Sensor Data"));
        requestTimer->start();
    }
}

void MainWindow::HostConnectionEvent(bool connected) {
    qDebug() << "Host Connection Event " << connected;
    if(connected) {
        ui->connectToServer->setText(QString("Connected To Server"));
    }
    else {
        ui->connectToServer->setText(QString("Connect To Server"));
    }
}

void MainWindow::receiveUpdate(QString leftFTSensor, QString middleFTSensor, QString rightFTSensor, QString leftFBSensor, QString middleFBSensor, QString rightFBSensor, QString leftSDSensor, QString rightSDSensor) {
    ui->topleftBrowser->append(leftFTSensor);
    ui->topmiddleBrowser->append(middleFTSensor);
    ui->toprightBrowser->append(rightFTSensor);
    ui->bottomleftBrowser->append(leftFBSensor);
    ui->bottommiddleBrowser->append(middleFBSensor);
    ui->bottomrightBrowser->append(rightFBSensor);
    ui->leftsideBrowser->append(leftSDSensor);
    ui->rightsideBrowser->append(rightSDSensor);
}

void MainWindow::on_connectToServer_clicked(){
    tcpSocket->connectToHost(ui->serverIPAddressInput->text(), ui->serverPortInput->text().toInt());
}

