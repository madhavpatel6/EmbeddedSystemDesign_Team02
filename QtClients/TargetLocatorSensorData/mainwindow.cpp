#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>
#include <algorithm>
#define REQUESTRATE_MS 100

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    requestTimer = new QTimer();
    requestTimer->setInterval(REQUESTRATE_MS);
    ui->setupUi(this);
    ui->labelTL->setText("mean\nstd\nvariance");
    ui->labelTM->setText("mean\nstd\nvariance");
    ui->labelTR->setText("mean\nstd\nvariance");
    ui->labelBL->setText("mean\nstd\nvariance");
    ui->labelBM->setText("mean\nstd\nvariance");
    ui->labelBR->setText("mean\nstd\nvariance");
    ui->labelRightSide->setText("mean\nstd\nvariance");
    ui->labelLeftSide->setText("mean\nstd\nvariance");

    ui->valuesTL->setText("0.00\n0.00\n0.00");
    ui->valuesTM->setText("0.00\n0.00\n0.00");
    ui->valuesTR->setText("0.00\n0.00\n0.00");
    ui->valuesBL->setText("0.00\n0.00\n0.00");
    ui->valuesBM->setText("0.00\n0.00\n0.00");
    ui->valuesBR->setText("0.00\n0.00\n0.00");
    ui->valuesLS->setText("0.00\n0.00\n0.00");
    ui->valuesRS->setText("0.00\n0.00\n0.00");
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
    if(valuesTL.size() == 1000) {
        valuesTL.pop_front();
        valuesTM.pop_front();
        valuesTR.pop_front();
        valuesBL.pop_front();
        valuesBM.pop_front();
        valuesBR.pop_front();
        valuesSL.pop_front();
        valuesSR.pop_front();
    }
    valuesTL.push_back(leftFTSensor.toFloat());
    valuesTM.push_back(middleFTSensor.toFloat());
    valuesTR.push_back(rightFTSensor.toFloat());
    valuesBL.push_back(leftFBSensor.toFloat());
    valuesBM.push_back(middleFBSensor.toFloat());
    valuesBR.push_back(rightFBSensor.toFloat());
    valuesSL.push_back(leftSDSensor.toFloat());
    valuesSR.push_back(rightSDSensor.toFloat());

    ui->topleftBrowser->append(leftFTSensor);
    ui->topmiddleBrowser->append(middleFTSensor);
    ui->toprightBrowser->append(rightFTSensor);
    ui->bottomleftBrowser->append(leftFBSensor);
    ui->bottommiddleBrowser->append(middleFBSensor);
    ui->bottomrightBrowser->append(rightFBSensor);
    ui->leftsideBrowser->append(leftSDSensor);
    ui->rightsideBrowser->append(rightSDSensor);

    updateStatistics(valuesTL, ui->valuesTL);
    updateStatistics(valuesTM, ui->valuesTM);
    updateStatistics(valuesTR, ui->valuesTR);
    updateStatistics(valuesBL, ui->valuesBL);
    updateStatistics(valuesBM, ui->valuesBM);
    updateStatistics(valuesBR, ui->valuesBR);
    updateStatistics(valuesSL, ui->valuesLS);
    updateStatistics(valuesSR, ui->valuesRS);

}

float MainWindow::computeMean(QVector<float> vec) {
    if(vec.size() == 0) {
        return 0;
    }
    float sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    return sum/vec.size();
}

float MainWindow::computeStd(QVector<float> vec) {
    if(vec.size() == 0) {
        return 0;
    }
    float sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    float mean = sum/vec.size();
    std::vector<double> diff(vec.size());
    std::transform(vec.begin(), vec.end(), diff.begin(),
                   std::bind2nd(std::minus<double>(), mean));
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    return std::sqrt(sq_sum / vec.size());
}

void MainWindow::updateStatistics(QVector<float> vec, QLabel* label) {
    label->clear();
    QString mean = QString::number(computeMean(vec),'f',3);
    float stddev_n = computeStd(vec);
    QString stddev = QString::number(stddev_n, 'f', 3);
    QString var = QString::number(std::pow(stddev_n,2),'f',3);
    label->setText(mean+'\n'+stddev+'\n'+var);
}

void MainWindow::on_connectToServer_clicked(){
    tcpSocket->connectToHost(ui->serverIPAddressInput->text(), ui->serverPortInput->text().toInt());
}


void MainWindow::on_pushButton_clicked()
{
    valuesTL.clear();
    valuesTM.clear();
    valuesTR.clear();
    valuesBL.clear();
    valuesBM.clear();
    valuesBR.clear();
    valuesSL.clear();
    valuesSR.clear();
    ui->topleftBrowser->clear();
    ui->topmiddleBrowser->clear();
    ui->toprightBrowser->clear();
    ui->bottomleftBrowser->clear();
    ui->bottommiddleBrowser->clear();
    ui->bottomrightBrowser->clear();
    ui->leftsideBrowser->clear();
    ui->rightsideBrowser->clear();
}
