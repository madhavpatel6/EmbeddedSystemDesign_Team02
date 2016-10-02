#include "targetalignment.h"
#include "ui_targetalignment.h"

#define REQUESTRATE_MS 200

targetAlignment::targetAlignment(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::targetAlignment)
{
    requestTimer = new QTimer();
    requestTimer->setInterval(REQUESTRATE_MS);
    ui->setupUi(this);
    tcpSocket = new ClientSocket();
    connect(requestTimer, SIGNAL(timeout()), tcpSocket, SLOT(alignmentRequestedSlot()));
    connect(tcpSocket, SIGNAL(sendAlignmentSignal(char,QString,QString,QString)), this, SLOT(updateAlignmentData(char,QString,QString,QString)));
    IR0RequestCounter = 0;
    IR1RequestCounter = 0;
    IR2RequestCounter = 0;

    // This is for the server connection status to display
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(bool)), this, SLOT(serverConnected(bool)));
}

targetAlignment::~targetAlignment()
{
    IR0RequestCounter = 0;
    IR1RequestCounter = 0;
    IR2RequestCounter = 0;
    delete requestTimer;
    delete tcpSocket;
    delete ui;
}

void targetAlignment::serverConnected(bool connected)
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

void targetAlignment::on_connectToServerButton_clicked()
{
    tcpSocket->connectToHost(ui->serverIPLineEdit->text(), ui->portLineEdit->text().toInt());
}

void targetAlignment::on_disconnectFromServerButton_clicked()
{
    ui->connectionStatusLabel->clear();
    ui->connectionStatusLabel->setText(QString("No"));
}

void targetAlignment::on_startSensingButton_clicked()
{
    if(requestTimer->isActive()) {
        ui->IR_0PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Stop"));
        ui->IR_1PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Stop"));
        ui->IR_2PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Stop"));
        requestTimer->stop();
    }
    else {
        ui->IR_0PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        ui->IR_1PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        ui->IR_2PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        requestTimer->start();
    }
}

void targetAlignment::updateAlignmentData(char source, QString alignmentData0, QString alignmentData1, QString alignmentData2)
{
    if(alignmentData0 == "0.00"){
        IR0RequestCounter = IR0RequestCounter;
    }
    else{
        IR0RequestCounter = IR0RequestCounter + 1;
    }
    if(alignmentData1 == "0.00"){
        IR1RequestCounter = IR1RequestCounter;
    }
    else{
        IR1RequestCounter = IR1RequestCounter + 1;
    }
    if(alignmentData2 == "0.00"){
        IR2RequestCounter = IR2RequestCounter;
    }
    else{
        IR2RequestCounter = IR2RequestCounter + 1;
    }
    switch(source){
        case TARGETGRABBER:
            ui->IR_0CountLabel->setText(QString::number(IR0RequestCounter));
            ui->IR_1CountLabel->setText(QString::number(IR1RequestCounter));
            ui->IR_2CountLabel->setText(QString::number(IR2RequestCounter));
            ui->IR_0PlainTextEdit->appendPlainText(alignmentData0);
            ui->IR_1PlainTextEdit->appendPlainText(alignmentData1);
            ui->IR_2PlainTextEdit->appendPlainText(alignmentData2);
            //ui->IR_0PlainTextEdit->appendPlainText(QString(source));
            //ui->IR_1PlainTextEdit->appendPlainText(QString(source));
            //ui->IR_2PlainTextEdit->appendPlainText(QString(source));
            break;
        case TARGETLOCATOR:
            IR0RequestCounter = IR0RequestCounter + 1;
            IR1RequestCounter = IR1RequestCounter + 1;
            IR2RequestCounter = IR2RequestCounter + 1;
            ui->IR_0CountLabel->setText(QString::number(IR0RequestCounter));
            ui->IR_1CountLabel->setText(QString::number(IR1RequestCounter));
            ui->IR_2CountLabel->setText(QString::number(IR2RequestCounter));
            ui->IR_0PlainTextEdit->appendPlainText(alignmentData0);
            ui->IR_1PlainTextEdit->appendPlainText(alignmentData1);
            ui->IR_2PlainTextEdit->appendPlainText(alignmentData2);
            ui->IR_0PlainTextEdit->appendPlainText(QString(source));
            ui->IR_1PlainTextEdit->appendPlainText(QString(source));
            ui->IR_2PlainTextEdit->appendPlainText(QString(source));
            break;
        case PATHFINDER:
            IR0RequestCounter = IR0RequestCounter + 1;
            IR1RequestCounter = IR1RequestCounter + 1;
            IR2RequestCounter = IR2RequestCounter + 1;
            ui->IR_0CountLabel->setText(QString::number(IR0RequestCounter));
            ui->IR_1CountLabel->setText(QString::number(IR1RequestCounter));
            ui->IR_2CountLabel->setText(QString::number(IR2RequestCounter));
            ui->IR_0PlainTextEdit->appendPlainText(alignmentData0);
            ui->IR_1PlainTextEdit->appendPlainText(alignmentData1);
            ui->IR_2PlainTextEdit->appendPlainText(alignmentData2);
            ui->IR_0PlainTextEdit->appendPlainText(QString(source));
            ui->IR_1PlainTextEdit->appendPlainText(QString(source));
            ui->IR_2PlainTextEdit->appendPlainText(QString(source));
            break;
        case SEARCHERMOVER:
            IR0RequestCounter = IR0RequestCounter + 1;
            IR1RequestCounter = IR1RequestCounter + 1;
            IR2RequestCounter = IR2RequestCounter + 1;
            ui->IR_0CountLabel->setText(QString::number(IR0RequestCounter));
            ui->IR_1CountLabel->setText(QString::number(IR1RequestCounter));
            ui->IR_2CountLabel->setText(QString::number(IR2RequestCounter));
            ui->IR_0PlainTextEdit->appendPlainText(alignmentData0);
            ui->IR_1PlainTextEdit->appendPlainText(alignmentData1);
            ui->IR_2PlainTextEdit->appendPlainText(alignmentData2);
            ui->IR_0PlainTextEdit->appendPlainText(QString(source));
            ui->IR_1PlainTextEdit->appendPlainText(QString(source));
            ui->IR_2PlainTextEdit->appendPlainText(QString(source));
            break;
    }
}

