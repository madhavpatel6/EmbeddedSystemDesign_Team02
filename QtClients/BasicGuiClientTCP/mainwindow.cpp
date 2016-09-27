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
    tcpSocket = new ClientSocket();
    connect(tcpSocket, SIGNAL(updateError(int)), this, SLOT(UpdateErrorCount(int)));
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

// If pressed then get comm stats from PICs
void MainWindow::on_commStatsButton_clicked()
{
    if(requestTimer->isActive()) {
        ui->commStatsButton->setText(QString("Request Comm Stats"));
        requestTimer->stop();
    }
    else {
        ui->commStatsButton->setText(QString("Stop Comm Stats"));
        requestTimer->start();
    }
}

void MainWindow::CommStatsRequestSent()
{
    commStatRequestCounter = commStatRequestCounter + 1;
    ui->RequestSentCount->setText(QString::number(commStatRequestCounter));
}


void MainWindow::on_connectToServer_clicked()
{
    tcpSocket->connectToHost(ui->serverIPAddressInput->text(), ui->serverPortInput->text().toInt());
}

void MainWindow::HostConnectionEvent(bool connected) {
    qDebug() << "Host Connection Event " << connected;
    if(connected)
        ui->connectToServer->setText(QString("Connected To Server"));
    else
        ui->connectToServer->setText(QString("Connect To Server"));
}
void MainWindow::UpdateErrorCount(int count) {
    ui->serverErrorVal->setText(QString::number(count));
}

void MainWindow::UpdateCommStats(char source, QString goodMsg, QString commError, QString reqRecv, QString resRecv, QString reqSent, QString resSent) {
    switch(source) {
        case SEARCHERMOVER:{
            ui->SMGoodMsg->setText(goodMsg);
            ui->SMCommError->setText(commError);
            if((commError.toInt() + goodMsg.toInt()) != 0) {
                ui->SMPercError->setText(QString::number(100.0*commError.toInt() / (commError.toInt() + goodMsg.toInt()),'g',3));
            }
            ui->SMReqRecv->setText(reqRecv);
            ui->SMResRecv->setText(resRecv);
            ui->SMReqSent->setText(reqSent);
            ui->SMResSent->setText(resSent);
            break;
        }
        case TARGETLOCATOR: {
            ui->TLGoodMsg->setText(goodMsg);
            ui->TLCommError->setText(commError);
            if((commError.toInt() + goodMsg.toInt()) != 0)
                ui->TLPercError->setText(QString::number(100.0*commError.toInt() / (commError.toInt() + goodMsg.toInt()),'g',3));
            ui->TLReqRecv->setText(reqRecv);
            ui->TLResRecv->setText(resRecv);
            ui->TLReqSent->setText(reqSent);
            ui->TLResSent->setText(resSent);
            break;
        }
        case PATHFINDER: {
            ui->PFGoodMsg->setText(goodMsg);
            ui->PFCommError->setText(commError);
            if((commError.toInt() + goodMsg.toInt()) != 0)
                ui->PFPercError->setText(QString::number(100.0*commError.toInt() / (commError.toInt() + goodMsg.toInt()),'g',3));
            ui->PFReqRecv->setText(reqRecv);
            ui->PFResRecv->setText(resRecv);
            ui->PFReqSent->setText(reqSent);
            ui->PFResSent->setText(resSent);
            break;
        }
        case TARGETGRABBER: {
            ui->TGGoodMsg->setText(goodMsg);
            ui->TGCommError->setText(commError);
            if((commError.toInt() + goodMsg.toInt()) != 0)
                ui->TGPercError->setText(QString::number(100.0*commError.toInt() / (commError.toInt() + goodMsg.toInt()),'g',3));
            ui->TGReqRecv->setText(reqRecv);
            ui->TGResRecv->setText(resRecv);
            ui->TGReqSent->setText(reqSent);
            ui->TGResSent->setText(resSent);
            break;
        }
    }
}
