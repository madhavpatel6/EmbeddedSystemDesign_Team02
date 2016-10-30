#include "mainwindow.h"
#include "ui_mainwindow.h"

#define REQUESTRATE_MS 200

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    requestTimer = new QTimer();
    requestTimer2 = new QTimer();
    requestTimer->setInterval(REQUESTRATE_MS);
    requestTimer2->setInterval(REQUESTRATE_MS);

    ui->setupUi(this);
    tcpSocket = new ClientSocket();

    connect(requestTimer, SIGNAL(timeout()), tcpSocket, SLOT(positionRequested()));
    connect(requestTimer2, SIGNAL(timeout()), tcpSocket, SLOT(lineLocationRequested()));
    connect(this, SIGNAL(pb_forwardClicked()), tcpSocket, SLOT(sendForwardCommand()));
    connect(this, SIGNAL(pb_backClicked()), tcpSocket, SLOT(sendBackCommand()));
    connect(this, SIGNAL(pb_leftClicked()), tcpSocket, SLOT(sendLeftCommand()));
    connect(this, SIGNAL(pb_rightClicked()), tcpSocket, SLOT(sendRightCommand()));
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(bool)), this, SLOT(HostConnectionEvent(bool)));
    connect(tcpSocket, SIGNAL(sendLocation(char,QString,QString)), this, SLOT(updateLocation(char,QString,QString)));
    connect(tcpSocket, SIGNAL(sendOrientation(char,QString)), this, SLOT(updateOrientation(char,QString)));
    connect(tcpSocket, SIGNAL(sendLineLocation(int)), this, SLOT(updateLineLocation(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete requestTimer;
    delete tcpSocket;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A) {
        tcpSocket->sendLeftCommand();
    } else if (event->key() == Qt::Key_D) {
        tcpSocket->sendRightCommand();
    } else if (event->key() == Qt::Key_W) {
        tcpSocket->sendForwardCommand();
    } else if (event->key() == Qt::Key_S) {
        tcpSocket->sendBackCommand();
    }
}


void MainWindow::on_pb_connectToServer_clicked()
{
    tcpSocket->connectToHost(ui->le_IPAddress->text(), ui->le_port->text().toInt());
}

void MainWindow::on_pb_requestPosition_clicked()
{
    if(requestTimer->isActive()) {
        ui->pb_requestPosition->setText(QString("Request Position"));
        requestTimer->stop();
    }
    else {
        ui->pb_requestPosition->setText(QString("Stop Requesting Position"));
        requestTimer->start();
    }
}

void MainWindow::HostConnectionEvent(bool connected) {
    qDebug() << "Host Connection Event " << connected;
    if(connected) {
        ui->pb_connectToServer->setText(QString("Connected To Server"));
    }
    else {
        ui->pb_connectToServer->setText(QString("Connect To Server"));
    }
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

void MainWindow::updateLineLocation(int location)
{
    ui->rb_lineSensor0->setChecked(location & 1);
    ui->rb_lineSensor1->setChecked(location & (1 << 1));
    ui->rb_lineSensor2->setChecked(location & (1 << 2));
    ui->rb_lineSensor3->setChecked(location & (1 << 3));
    ui->rb_lineSensor4->setChecked(location & (1 << 4));
    ui->rb_lineSensor5->setChecked(location & (1 << 5));
    ui->rb_lineSensor6->setChecked(location & (1 << 6));
    ui->rb_lineSensor7->setChecked(location & (1 << 7));
}

void MainWindow::on_pb_forward_clicked()
{
    emit pb_forwardClicked();
}

void MainWindow::on_pb_back_clicked()
{
    emit pb_backClicked();
}

void MainWindow::on_pb_left_clicked()
{
    emit pb_leftClicked();
}

void MainWindow::on_pb_right_clicked()
{
    emit pb_rightClicked();
}

void MainWindow::on_pb_requestLineSensor_clicked()
{
    if(requestTimer2->isActive()) {
        ui->pb_requestLineSensor->setText(QString("Request"));
        requestTimer2->stop();
    }
    else {
        ui->pb_requestLineSensor->setText(QString("Stop"));
        requestTimer2->start();
    }
}
