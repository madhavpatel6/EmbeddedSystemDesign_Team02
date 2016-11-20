#include "mainwindow.h"
#include "ui_mainwindow.h"

#define REQUESTRATE_MS 200
#define M_PI 3.14159265358979323846

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    requestTimer = new QTimer();
    requestTimer->setInterval(REQUESTRATE_MS);
    ui->setupUi(this);
    tcpSocket = new ClientSocket();
//    connect(tcpSocket, SIGNAL(updateError(int)), this, SLOT(UpdateErrorCount(int)));
    connect(requestTimer, SIGNAL(timeout()), this, SLOT(sendPosition()));
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(bool)), this, SLOT(HostConnectionEvent(bool)));
    connect(tcpSocket, SIGNAL(updateProximity(QString,QString,QString)), this, SLOT(updateProximity(QString,QString,QString)));
//    connect(tcpSocket, SIGNAL(sentCommStatSignal()), this, SLOT(CommStatsRequestSent()));
//    connect(tcpSocket, SIGNAL(sendCommStat(char,QString,QString,QString,QString,QString,QString)), this, SLOT(UpdateCommStats(char,QString,QString,QString,QString,QString,QString)));
    commStatRequestCounter = 0;
    requestProximity = false;
    respondPosition = false;

}

MainWindow::~MainWindow()
{
    delete ui;
    delete requestTimer;
    delete tcpSocket;
}


void MainWindow::on_commStatsButton_clicked()
{
    if(respondPosition) {
        ui->commStatsButton->setText(QString("Start Sending Position"));
        if(!requestProximity)
            requestTimer->stop();
        respondPosition = false;
    }
    else {
        respondPosition = true;
        itr = 0;
        deltax = ui->endX->text().toFloat() - ui->startX->text().toFloat();
        deltay = ui->endY->text().toFloat() - ui->startY->text().toFloat();
        deltaangle = ui->endAngle->text().toFloat() - ui->startAngle->text().toFloat();
        startX = ui->startX->text().toFloat();
        startY = ui->startY->text().toFloat();
        startAngle = ui->startAngle->text().toFloat();
        change = 0;
        numberofiterations = 0;
        if(deltaangle == 0) {
            change = sqrt(pow(deltax,2) + pow(deltay,2));
            numberofiterations = change / ui->speed->text().toFloat();
        }
        else {
            numberofiterations = abs(deltaangle / ui->speed->text().toFloat());
        }


        qDebug() << "change " << change;
        qDebug() << "numitr " << numberofiterations;
        ui->commStatsButton->setText(QString("Pause Sending Position"));
        requestTimer->start();
    }
    qDebug() << respondPosition;
}

void MainWindow::sendPosition()
{
    if(respondPosition) {
        float aa = atan2(ui->endY->text().toFloat() - startY, ui->endX->text().toFloat() - startX)*180/M_PI;
        qDebug() << aa;
        QString x = QString::number(startX + change*cos(aa*M_PI/180)*itr/numberofiterations);
        QString y = QString::number(startY + change*sin(aa*M_PI/180)*itr/numberofiterations);
        QString angle = QString::number(startAngle + deltaangle*itr/numberofiterations);
        ui->currAngle->setText(angle);
        ui->currX->setText(x);
        ui->currY->setText(y);
        tcpSocket->sendPositionUpdate(x,y,angle);
        itr++;
        ui->progressBar->setValue(1.0*itr/numberofiterations*100);
        if(numberofiterations < itr) {
            if(!requestProximity)
                requestTimer->stop();
            respondPosition = false;
           ui->commStatsButton->setText(QString("Start Sending Position"));
           requestTimer->stop();
           itr = 0;
        }
    }
    if(requestProximity) {
        tcpSocket->sendProximityRequest();
    }
}

void MainWindow::on_connectToServer_clicked()
{
    tcpSocket->connectToHost(ui->serverIPAddressInput->text(), ui->serverPortInput->text().toInt());
}

void MainWindow::updateProximity(QString farLeft,QString middle, QString farRight) {
    ui->farLeft->setText(farLeft);
    ui->middle->setText(middle);
    ui->farRight->setText(farRight);
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

void MainWindow::on_StopPositionResponse_released()
{

}

void MainWindow::on_pushButton_released()
{
    if(requestProximity){
        ui->pushButton->setText(QString("Start Requesting Proximity Information"));
        if(!respondPosition )
            requestTimer->stop();
        requestProximity = false;
    }
    else {
        requestProximity = true;
        requestTimer->start();
        ui->pushButton->setText(QString("Stop Requesting Proximity Information"));
    }
}
