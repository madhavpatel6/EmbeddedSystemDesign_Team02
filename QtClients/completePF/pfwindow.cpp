#include "pfwindow.h"
#include "ui_pfwindow.h"

#define REQUESTRATE_MS 200

pfWindow::pfWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pfWindow)
{
    requestTimer = new QTimer();
    requestTimer->setInterval(REQUESTRATE_MS);
    sendProxTimer = new QTimer();
    sendProxTimer->setInterval(REQUESTRATE_MS);
    varianceFormObj = new varianceForm();
    req_respFormObj = new req_respForm();
    tg_req_respFormObj = new tg_req_respForm();
    ui->setupUi(this);
    tcpSocket = new ClientSocket();
    connect(requestTimer, SIGNAL(timeout()), tcpSocket, SLOT(alignmentRequestedSlot()));
    connect(sendProxTimer, SIGNAL(timeout()), this, SLOT(sendProxSlot()));
    connect(tcpSocket, SIGNAL(sendAlignmentSignal(char,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)), this, SLOT(updateAlignmentData(char,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));
    connect(tcpSocket,SIGNAL(sendAcquiredSignal(QString)),this,SLOT(updateAcquiredSlot(QString)));
    connect(this,SIGNAL(varianceSignal(char,QString,QString,QString,QString,QString)),varianceFormObj,SLOT(updateVarianceData(char,QString,QString,QString,QString,QString)));
    connect(tcpSocket,SIGNAL(sendCurReqSignal(QString)),req_respFormObj,SLOT(updateCurReqSlot(QString)));
    connect(tcpSocket,SIGNAL(sendCurRespSignal(QString)),req_respFormObj,SLOT(updateCurRespSlot(QString)));
    connect(tcpSocket,SIGNAL(sendtgCurReqSignal(QString)),tg_req_respFormObj,SLOT(updatetgCurReqSlot(QString)));
    connect(tcpSocket,SIGNAL(sendtgCurRespSignal(QString)),tg_req_respFormObj,SLOT(updatetgCurRespSlot(QString)));
    IR0RequestCounter = 0;
    IR1RequestCounter = 0;
    IR2RequestCounter = 0;
    IR3RequestCounter = 0;
    IR4RequestCounter = 0;
    proximitySliderValue = 0;
    sendMovementClickedBool = false;
    sendOutsideArenaClickedBool = false;
    requestTargetAcquiredBool = false;

    // This is for the server connection status to display
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(bool)), this, SLOT(serverConnected(bool)));
}

pfWindow::~pfWindow()
{
    IR0RequestCounter = 0;
    IR1RequestCounter = 0;
    IR2RequestCounter = 0;
    IR3RequestCounter = 0;
    IR4RequestCounter = 0;
    proximitySliderValue = 0;
    sendMovementClickedBool = false;
    sendOutsideArenaClickedBool = false;
    requestTargetAcquiredBool = false;
    delete requestTimer;
    delete sendProxTimer;
    delete varianceFormObj;
    delete req_respFormObj;
    delete tg_req_respFormObj;
    delete tcpSocket;
    delete ui;
}

void pfWindow::serverConnected(bool connected)
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

void pfWindow::on_proximitySlider_valueChanged(int value)
{
    proximitySliderValue = value;
    ui->proxlcdNumber->display(QString::number(proximitySliderValue));
}
void pfWindow::sendProxSlot()
{
    tcpSocket->sendProximity(proximitySliderValue);
}

void pfWindow::on_connectToServerButton_clicked()
{
    tcpSocket->connectToHost(ui->serverIPLineEdit->text(), ui->portLineEdit->text().toInt());
}

void pfWindow::on_disconnectFromServerButton_clicked()
{
    ui->connectionStatusLabel->clear();
    ui->connectionStatusLabel->setText(QString("No"));
}

void pfWindow::on_startSensingButton_clicked()
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
        ui->IR_3PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Stop"));
        ui->IR_3_bool_textEdit->setTextColor(QColor("black"));
        ui->IR_3_bool_textEdit->append(QString("Sensor Button Clicked; Stop"));
        ui->IR_4PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Stop"));
        ui->IR_4_bool_textEdit->setTextColor(QColor("black"));
        ui->IR_4_bool_textEdit->append(QString("Sensor Button Clicked; Stop"));
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
        ui->IR_3PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        ui->IR_3_bool_textEdit->setTextColor(QColor("black"));
        ui->IR_3_bool_textEdit->append(QString("Sensor Button Clicked; Start"));
        ui->IR_4PlainTextEdit->appendPlainText(QString("Sensor Button Clicked; Start"));
        ui->IR_4_bool_textEdit->setTextColor(QColor("black"));
        ui->IR_4_bool_textEdit->append(QString("Sensor Button Clicked; Start"));
        requestTimer->start();
    }
}

void pfWindow::updateAlignmentData(char source, QString alignmentData0, QString alignmentData1, QString alignmentData2, QString alignmentData3, QString alignmentData4, QString distanceFromTarget, QString angleToTarget, QString alignmentBool_0, QString alignmentBool_1, QString alignmentBool_2, QString alignmentBool_3, QString alignmentBool_4, QString alignmentGrabberBool, QString alignmentInfo)
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
    if(alignmentData3 == "0.00"){
        IR3RequestCounter = IR3RequestCounter;
    }
    else{
        IR3RequestCounter = IR3RequestCounter + 1;
    }
    if(alignmentData4 == "0.00"){
        IR4RequestCounter = IR4RequestCounter;
    }
    else{
        IR4RequestCounter = IR4RequestCounter + 1;
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
    QTextCursor cursorIR_3Plain = ui->IR_3PlainTextEdit->textCursor();
    QTextCursor cursorIR_4Plain = ui->IR_4PlainTextEdit->textCursor();
    QTextCursor cursorIR_0_bool = ui->IR_0_bool_textEdit->textCursor();
    QTextCursor cursorIR_1_bool = ui->IR_1_bool_textEdit->textCursor();
    QTextCursor cursorIR_2_bool = ui->IR_2_bool_textEdit->textCursor();
    QTextCursor cursorIR_3_bool = ui->IR_3_bool_textEdit->textCursor();
    QTextCursor cursorIR_4_bool = ui->IR_4_bool_textEdit->textCursor();
    switch(source){
        case TARGETGRABBER:
            ui->IR_0CountLabel->setText(QString::number(IR0RequestCounter));
            ui->IR_1CountLabel->setText(QString::number(IR1RequestCounter));
            ui->IR_2CountLabel->setText(QString::number(IR2RequestCounter));
            ui->IR_3CountLabel->setText(QString::number(IR3RequestCounter));
            ui->IR_4CountLabel->setText(QString::number(IR4RequestCounter));
            ui->Alignment_InfoLabel->setText(QString(alignmentInfo));
            ui->angleLcdNumber->display(angleToTarget.toFloat());
            ui->distanceFromTargetLcdNumber->display(distanceFromTarget.toFloat());
            emit varianceSignal(source, alignmentData0, alignmentData1, alignmentData2, alignmentData3, alignmentData4);
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

            if(alignmentBool_3 == "1"){
                ui->IR_3PlainTextEdit->appendPlainText(alignmentData3);
                ui->IR_3_bool_textEdit->setTextColor(QColor("green"));
                ui->IR_3_bool_textEdit->append(QString("True"));
                cursorIR_3Plain.movePosition(QTextCursor::End);
                cursorIR_3_bool.movePosition(QTextCursor::End);
                ui->IR_3PlainTextEdit->setTextCursor(cursorIR_3Plain);
                ui->IR_3_bool_textEdit->setTextCursor(cursorIR_3_bool);
            }
            else{
                ui->IR_3PlainTextEdit->appendPlainText(alignmentData3);
                ui->IR_3_bool_textEdit->setTextColor(QColor("red"));
                ui->IR_3_bool_textEdit->append(QString("False"));
                cursorIR_3Plain.movePosition(QTextCursor::End);
                cursorIR_3_bool.movePosition(QTextCursor::End);
                ui->IR_3PlainTextEdit->setTextCursor(cursorIR_3Plain);
                ui->IR_3_bool_textEdit->setTextCursor(cursorIR_3_bool);
            }

            if(alignmentBool_4 == "1"){
                ui->IR_4PlainTextEdit->appendPlainText(alignmentData4);
                ui->IR_4_bool_textEdit->setTextColor(QColor("green"));
                ui->IR_4_bool_textEdit->append(QString("True"));
                cursorIR_4Plain.movePosition(QTextCursor::End);
                cursorIR_4_bool.movePosition(QTextCursor::End);
                ui->IR_4PlainTextEdit->setTextCursor(cursorIR_4Plain);
                ui->IR_4_bool_textEdit->setTextCursor(cursorIR_4_bool);
            }
            else{
                ui->IR_4PlainTextEdit->appendPlainText(alignmentData4);
                ui->IR_4_bool_textEdit->setTextColor(QColor("red"));
                ui->IR_4_bool_textEdit->append(QString("False"));
                cursorIR_4Plain.movePosition(QTextCursor::End);
                cursorIR_4_bool.movePosition(QTextCursor::End);
                ui->IR_4PlainTextEdit->setTextCursor(cursorIR_4Plain);
                ui->IR_4_bool_textEdit->setTextCursor(cursorIR_4_bool);
            }
            break;
        default:
                qDebug() << "BROKEN" << endl;
                break;
    }
}

// This will tell if the target grabber has acquired the target or not
void pfWindow::updateAcquiredSlot(QString targetAcuiredBool){
    if(targetAcuiredBool == "1"){
        ui->target_acquired_bool_label->setText(QString("True"));
    }
    else{
        ui->target_acquired_bool_label->setText(QString("False"));
    }
}


void pfWindow::on_proximityPushButton_clicked()
{
    if(sendProxTimer->isActive()) {
        ui->prox_sent_bool_label->setText(QString("False"));
        sendProxTimer->stop();
    }
    else{
        ui->prox_sent_bool_label->setText(QString("True"));
        sendProxTimer->start();
    }
}

// This displays the variance of the IR's when the menu action is triggered
void pfWindow::on_actionCalculate_Variance_triggered()
{
    varianceFormObj->show();
}

void pfWindow::on_sendMovementPushButton_clicked()
{
    if(sendMovementClickedBool == true){
        tcpSocket->sendMovement(false);
        ui->movement_bool_label->setText(QString("False"));
        sendMovementClickedBool = false;
    }
    else{
        tcpSocket->sendMovement(true);
        ui->movement_bool_label->setText(QString("True"));
        sendMovementClickedBool = true;
    }

}

void pfWindow::on_sendOutsideofArenaPushButton_clicked()
{
    if(sendOutsideArenaClickedBool == true){
        tcpSocket->sendOutsideArena(false);
        ui->outside_of_arena_bool_label->setText(QString("False"));
        sendOutsideArenaClickedBool = false;
    }
    else{
        tcpSocket->sendOutsideArena(true);
        ui->outside_of_arena_bool_label->setText(QString("True"));
        sendOutsideArenaClickedBool = true;
    }

}

// This displays the requests and responses from the PF when the menu
// bar action is selected
void pfWindow::on_actionRequests_Responses_triggered()
{
    req_respFormObj->show();
}

// This sends a request to the target grabber asking if it has acquired the target
void pfWindow::on_requestTargetAcquiredpushButton_clicked()
{
    if(requestTargetAcquiredBool == true){
        tcpSocket->targetAcquiredRequested(false);
        ui->target_Acquired_Request_Sent_Bool_label->setText(QString("False"));
        requestTargetAcquiredBool = false;
    }
    else{
        tcpSocket->targetAcquiredRequested(true);
        ui->target_Acquired_Request_Sent_Bool_label->setText(QString("True"));
        requestTargetAcquiredBool = true;
    }
}

// This will display the requests and responses coming from the target
// grabber based on the menu action
void pfWindow::on_actionTG_Requests_and_Responses_triggered()
{
    tg_req_respFormObj->show();
}
