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
    ui->labelLS->setText("mean\nstd\nvariance");
    ui->labelTR->setText("mean\nstd\nvariance");
    ui->labelBL->setText("mean\nstd\nvariance");
    ui->labelRS->setText("mean\nstd\nvariance");
    ui->labelBR->setText("mean\nstd\nvariance");
    ui->labelUL->setText("mean\nstd\nvariance");
    ui->labelUM->setText("mean\nstd\nvariance");
    ui->labelUR->setText("mean\nstd\nvariance");

    ui->valuesTL->setText("0.00\n0.00\n0.00");
    ui->valuesLS->setText("0.00\n0.00\n0.00");
    ui->valuesTR->setText("0.00\n0.00\n0.00");
    ui->valuesBL->setText("0.00\n0.00\n0.00");
    ui->valuesRS->setText("0.00\n0.00\n0.00");
    ui->valuesBR->setText("0.00\n0.00\n0.00");
    ui->valuesUL->setText("0.00\n0.00\n0.00");
    ui->valuesUM->setText("0.00\n0.00\n0.00");
    ui->valuesUR->setText("0.00\n0.00\n0.00");
    tcpSocket  = new ClientSocket();
    connect(requestTimer, SIGNAL(timeout()), tcpSocket, SLOT(sendRequest()));
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(bool)), this, SLOT(HostConnectionEvent(bool)));
    connect(tcpSocket, SIGNAL(sendUpdate(QString,QString,QString,QString,QString,QString,QString,QString,QString)), this, SLOT(receiveUpdate(QString,QString,QString,QString,QString,QString,QString,QString,QString)));
    connect(tcpSocket, SIGNAL(updateTime(QString)), this, SLOT(receiveTime(QString)));
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

void MainWindow::receiveUpdate(QString leftFTSensor, QString rightFTSensor, QString leftFBSensor, QString rightFBSensor, QString leftUltra, QString middleUltra, QString rightUltra, QString leftSideUltra, QString rightSideUltra) {
    if(valuesTL.size() == 1000) {
        valuesTL.pop_front();
        valuesUL.pop_front();
        valuesTR.pop_front();
        valuesBL.pop_front();
        valuesUR.pop_front();
        valuesUM.pop_front();
        valuesLS.pop_front();
        valuesRS.pop_front();
        valuesBR.pop_front();
    }
    valuesTL.push_back(leftFTSensor.toFloat());
    valuesTR.push_back(rightFTSensor.toFloat());
    valuesBL.push_back(leftFBSensor.toFloat());
    valuesBR.push_back(rightFBSensor.toFloat());
    valuesUL.push_back(leftUltra.toFloat());
    valuesUM.push_back(middleUltra.toFloat());
    valuesUR.push_back(rightUltra.toFloat());
    valuesLS.push_back(leftSideUltra.toFloat());
    valuesRS.push_back(rightSideUltra.toFloat());

    ui->topleftBrowser->append(leftFTSensor);
    ui->toprightBrowser->append(rightFTSensor);
    ui->bottomleftBrowser->append(leftFBSensor);
    ui->bottomrightBrowser->append(rightFBSensor);
    ui->ultraLeftBrowser->append(leftUltra);
    ui->ultraMiddleBrowser->append(middleUltra);
    ui->ultraRightBrowser->append(rightUltra);
    ui->ultraLeftSideBrowser->append(leftSideUltra);
    ui->ultraRightSideBrowser->append(rightSideUltra);

    updateStatistics(valuesTL, ui->valuesTL);
    updateStatistics(valuesTR, ui->valuesTR);
    updateStatistics(valuesBL, ui->valuesBL);
    updateStatistics(valuesBR, ui->valuesBR);
    updateStatistics(valuesUL, ui->valuesUL);
    updateStatistics(valuesUM, ui->valuesUM);
    updateStatistics(valuesUR, ui->valuesUR);
    updateStatistics(valuesLS, ui->valuesLS);
    updateStatistics(valuesRS, ui->valuesRS);


}

void MainWindow::receiveTime(QString time){

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
    valuesUL.clear();
    valuesTR.clear();
    valuesBL.clear();
    valuesUM.clear();
    valuesBR.clear();
    valuesUR.clear();
    valuesLS.clear();
    valuesRS.clear();

    ui->topleftBrowser->clear();
    ui->toprightBrowser->clear();
    ui->bottomleftBrowser->clear();
    ui->bottomrightBrowser->clear();
    ui->ultraLeftBrowser->clear();
    ui->ultraMiddleBrowser->clear();
    ui->ultraRightBrowser->clear();
    ui->ultraLeftSideBrowser->clear();
    ui->ultraRightSideBrowser->clear();

}
