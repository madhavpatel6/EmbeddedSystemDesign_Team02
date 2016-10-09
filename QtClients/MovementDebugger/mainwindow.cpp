#include "mainwindow.h"
#include "ui_mainwindow.h"

#define REQUESTRATE_MS 200

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    requestTimer = new QTimer();
    requestTimer->setInterval(REQUESTRATE_MS);
    ui->setupUi(this);
    tcpSocket = new ClientSocket();
    connect(requestTimer, SIGNAL(timeout()), tcpSocket, SLOT(positionRequested()));
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(bool)), this, SLOT(HostConnectionEvent(bool)));
    connect(tcpSocket, SIGNAL(sentPositionSignal()), this, SLOT(positionRequestSent()));
    connect(tcpSocket, SIGNAL(sendLocation(char,QString,QString)), this, SLOT(updateLocation(char,QString,QString)));
    connect(tcpSocket, SIGNAL(sendOrientation(char,QString)), this, SLOT(updateOrientation(char,QString)));
    requestCounter = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete requestTimer;
    delete tcpSocket;
}


void MainWindow::on_btn_connectToServer_clicked()
{
    tcpSocket->connectToHost(ui->le_IPAddress->text(), ui->le_port->text().toInt());
}

void MainWindow::on_btn_requestPosition_clicked()
{
    if(requestTimer->isActive()) {
        ui->btn_requestPosition->setText(QString("Request Position"));
        requestTimer->stop();
    }
    else {
        ui->btn_requestPosition->setText(QString("Stop Requesting Position"));
        requestTimer->start();
    }
}

void MainWindow::HostConnectionEvent(bool connected) {
    qDebug() << "Host Connection Event " << connected;
    if(connected) {
        ui->btn_connectToServer->setText(QString("Connected To Server"));
    }
    else {
        ui->btn_connectToServer->setText(QString("Connect To Server"));
    }
}

void MainWindow::positionRequestSent()
{
    requestCounter += 1;
    ui->lbl_requestCounter->setText(QString::number(requestCounter));
}

void MainWindow::updateLocation(char source, QString x, QString y)
{
    switch(source) {
        case SEARCHERMOVER: {
            ui->lbl_x_positionVal->setText(x);
            ui->lbl_y_positionVal->setText(y);
            break;
        }
        default: {
            break;
        }
    }
}

void MainWindow::updateOrientation(char source, QString orientation)
{
    switch(source) {
        case SEARCHERMOVER: {
            ui->lbl_orientationVal->setText(orientation);
            break;
        }
        default: {
            break;
        }
    }
}
