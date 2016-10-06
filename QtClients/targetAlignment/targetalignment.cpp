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
    connect(tcpSocket, SIGNAL(sendAlignmentSignal(char,QString,QString,QString,QString,QString,QString,QString,QString)), this, SLOT(updateAlignmentData(char,QString,QString,QString,QString,QString,QString,QString,QString)));
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
        ui->IR_0_bool_textEdit->setTextColor(QColor("black"));
        ui->IR_0_bool_textEdit->append(QString("Sensor Button Clicked; Stop"));
        ui->IR_1PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Stop"));
        ui->IR_1_bool_textEdit->setTextColor(QColor("black"));
        ui->IR_1_bool_textEdit->append(QString("Sensor Button Clicked; Stop"));
        ui->IR_2PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Stop"));
        ui->IR_2_bool_textEdit->setTextColor(QColor("black"));
        ui->IR_2_bool_textEdit->append(QString("Sensor Button Clicked; Stop"));
        requestTimer->stop();
    }
    else {
        ui->IR_0PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        ui->IR_0_bool_textEdit->setTextColor(QColor("black"));
        ui->IR_0_bool_textEdit->append(QString("Sensor Button Clicked; Start"));
        ui->IR_1PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        ui->IR_1_bool_textEdit->setTextColor(QColor("black"));
        ui->IR_1_bool_textEdit->append(QString("Sensor Button Clicked; Start"));
        ui->IR_2PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        ui->IR_2_bool_textEdit->setTextColor(QColor("black"));
        ui->IR_2_bool_textEdit->append(QString("Sensor Button Clicked; Start"));
        requestTimer->start();
    }
}

void targetAlignment::updateAlignmentData(char source, QString alignmentData0, QString alignmentData1, QString alignmentData2, QString alignmentBool_0, QString alignmentBool_1, QString alignmentBool_2, QString alignmentGrabberBool, QString alignmentInfo)
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
    if(alignmentGrabberBool == "1"){
        ui->Grabber_AlignedLabel->setText(QString("Yes"));
    }
    else{
        ui->Grabber_AlignedLabel->setText(QString("No"));
    }
    QTextCursor cursorIR_0Plain = ui->IR_0PlainTextEdit->textCursor();
    QTextCursor cursorIR_1Plain = ui->IR_1PlainTextEdit->textCursor();
    QTextCursor cursorIR_2Plain = ui->IR_2PlainTextEdit->textCursor();
    QTextCursor cursorIR_0_bool = ui->IR_0_bool_textEdit->textCursor();
    QTextCursor cursorIR_1_bool = ui->IR_1_bool_textEdit->textCursor();
    QTextCursor cursorIR_2_bool = ui->IR_2_bool_textEdit->textCursor();
    switch(source){
        case TARGETGRABBER:
            ui->IR_0CountLabel->setText(QString::number(IR0RequestCounter));
            ui->IR_1CountLabel->setText(QString::number(IR1RequestCounter));
            ui->IR_2CountLabel->setText(QString::number(IR2RequestCounter));
            ui->Alignment_InfoLabel->setText(QString(alignmentInfo));

            if(alignmentBool_0 == "1"){
                ui->IR_0PlainTextEdit->appendPlainText(alignmentData0);
                ui->IR_0_bool_textEdit->setTextColor(QColor("green"));
                ui->IR_0_bool_textEdit->append(QString("True"));
                cursorIR_0Plain.movePosition(QTextCursor::End);
                cursorIR_0_bool.movePosition(QTextCursor::End);
                ui->IR_0PlainTextEdit->setTextCursor(cursorIR_0Plain);
                ui->IR_0_bool_textEdit->setTextCursor(cursorIR_0_bool);
            }
            else{
                ui->IR_0PlainTextEdit->appendPlainText(alignmentData0);
                ui->IR_0_bool_textEdit->setTextColor(QColor("red"));
                ui->IR_0_bool_textEdit->append(QString("False"));
                cursorIR_0Plain.movePosition(QTextCursor::End);
                cursorIR_0_bool.movePosition(QTextCursor::End);
                ui->IR_0PlainTextEdit->setTextCursor(cursorIR_0Plain);
                ui->IR_0_bool_textEdit->setTextCursor(cursorIR_0_bool);
            }


            if(alignmentBool_1 == "1"){
                ui->IR_1PlainTextEdit->appendPlainText(alignmentData1);
                ui->IR_1_bool_textEdit->setTextColor(QColor("green"));
                ui->IR_1_bool_textEdit->append(QString("True"));
                cursorIR_1Plain.movePosition(QTextCursor::End);
                cursorIR_1_bool.movePosition(QTextCursor::End);
                ui->IR_1PlainTextEdit->setTextCursor(cursorIR_1Plain);
                ui->IR_1_bool_textEdit->setTextCursor(cursorIR_1_bool);
            }
            else{
                ui->IR_1PlainTextEdit->appendPlainText(alignmentData1);
                ui->IR_1_bool_textEdit->setTextColor(QColor("red"));
                ui->IR_1_bool_textEdit->append(QString("False"));
                cursorIR_1Plain.movePosition(QTextCursor::End);
                cursorIR_1_bool.movePosition(QTextCursor::End);
                ui->IR_1PlainTextEdit->setTextCursor(cursorIR_1Plain);
                ui->IR_1_bool_textEdit->setTextCursor(cursorIR_1_bool);
            }


            if(alignmentBool_2 == "1"){
                ui->IR_2PlainTextEdit->appendPlainText(alignmentData2);
                ui->IR_2_bool_textEdit->setTextColor(QColor("green"));
                ui->IR_2_bool_textEdit->append(QString("True"));
                cursorIR_2Plain.movePosition(QTextCursor::End);
                cursorIR_2_bool.movePosition(QTextCursor::End);
                ui->IR_2PlainTextEdit->setTextCursor(cursorIR_2Plain);
                ui->IR_2_bool_textEdit->setTextCursor(cursorIR_2_bool);
            }
            else{
                ui->IR_2PlainTextEdit->appendPlainText(alignmentData2);
                ui->IR_2_bool_textEdit->setTextColor(QColor("red"));
                ui->IR_2_bool_textEdit->append(QString("False"));
                cursorIR_2Plain.movePosition(QTextCursor::End);
                cursorIR_2_bool.movePosition(QTextCursor::End);
                ui->IR_2PlainTextEdit->setTextCursor(cursorIR_2Plain);
                ui->IR_2_bool_textEdit->setTextCursor(cursorIR_2_bool);
            }
            break;
        case TARGETLOCATOR:
            ui->IR_0CountLabel->setText(QString::number(IR0RequestCounter));
            ui->IR_1CountLabel->setText(QString::number(IR1RequestCounter));
            ui->IR_2CountLabel->setText(QString::number(IR2RequestCounter));
            ui->Alignment_InfoLabel->setText(QString(alignmentInfo));
            ui->IR_0PlainTextEdit->appendPlainText(alignmentData0);
            ui->IR_1PlainTextEdit->appendPlainText(alignmentData1);
            ui->IR_2PlainTextEdit->appendPlainText(alignmentData2);
            ui->IR_0PlainTextEdit->appendPlainText(QString(source));
            ui->IR_1PlainTextEdit->appendPlainText(QString(source));
            ui->IR_2PlainTextEdit->appendPlainText(QString(source));
            break;
        case PATHFINDER:
            ui->Alignment_InfoLabel->setText(QString(alignmentInfo));
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
            ui->Alignment_InfoLabel->setText(QString(alignmentInfo));
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

