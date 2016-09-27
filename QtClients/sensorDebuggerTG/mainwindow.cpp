#include "mainwindow.h"
#include "ui_mainwindow.h"

#define REQUESTRATE_MS 175

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    requestTimer = new QTimer();
    requestTimer->setInterval(REQUESTRATE_MS);
    ui->setupUi(this);
    tcpSocket = new ClientSocket();
    connect(requestTimer, SIGNAL(timeout()), tcpSocket, SLOT(alignmentRequestedSlot()));
    connect(tcpSocket, SIGNAL(sendAlignmentSignal(char,QString)), this, SLOT(updateAlignmentData(char,QString)));
    sensorRequestCounter = 0;

    // This is for the server connection status to display
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(bool)), this, SLOT(serverConnected(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete requestTimer;
    delete tcpSocket;
}

void MainWindow::serverConnected(bool connected)
{
    if(connected == true){
       ui->connectionStatusLabel->clear();
       ui->connectionStatusLabel->setText(QString("Yes"));
    }
    else{
        ui->connectionStatusLabel->clear();
        ui->connectionStatusLabel->setText(QString("No"));
    }
}

void MainWindow::on_connectToServerButton_clicked()
{
    tcpSocket->connectToHost(ui->serverIPLineEdit->text(), ui->portLineEdit->text().toInt());
}

void MainWindow::on_disconnectFromServerButton_clicked()
{
    ui->connectionStatusLabel->clear();
    ui->connectionStatusLabel->setText(QString("No"));
}

void MainWindow::on_startSensingButton_clicked()
{
    if(requestTimer->isActive()) {
        ui->IR_0PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        ui->IR_1PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        ui->IR_2PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        requestTimer->stop();
    }
    else {
        ui->IR_0PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Stop"));
        ui->IR_1PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Stop"));
        ui->IR_2PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Stop"));
        requestTimer->start();
    }
}

void MainWindow::updateAlignmentData(char source, QString alignmentData)
{
    if(source == TARGETGRABBER){
        sensorRequestCounter = sensorRequestCounter + 1;
        ui->IR_0CountLabel->setText(QString::number(sensorRequestCounter));
        ui->IR_1CountLabel->setText(QString::number(sensorRequestCounter));
        ui->IR_2CountLabel->setText(QString::number(sensorRequestCounter));
        ui->IR_0PlainTextEdit->appendPlainText(alignmentData);
        ui->IR_1PlainTextEdit->appendPlainText(alignmentData);
        ui->IR_2PlainTextEdit->appendPlainText(alignmentData);
    }
}
