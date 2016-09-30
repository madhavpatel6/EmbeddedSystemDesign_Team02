#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    socket = new ClientSocket();

    QJsonObject requests = initialization::getConfig("requests.json");

    QJsonObject responses = initialization::getConfig("responses.json");


    /* The following code read the requests and responses from file and creates checkboxes for them */
    QCheckBox* reqBox[requests.size()];
    QCheckBox* resBox[responses.size()];
    int count = 0;
    for(auto i = requests.begin(); i != requests.end(); i++){
        reqBox[count] = new QCheckBox();
        reqBox[count]->setText(i.key());
        ui->requestsEnable->addWidget(reqBox[count]);

        connect (reqBox[count], SIGNAL(clicked()), this, SLOT(on_reqCheckBoxClicked()));
        count++;
    }
    count = 0;
    for(auto i = responses.begin(); i != responses.end(); i++){
        resBox[count] = new QCheckBox();
        resBox[count]->setText(i.key());
        ui->responsesEnable->addWidget(resBox[count]);

        connect (resBox[count], SIGNAL(clicked()), this, SLOT(on_resCheckBoxClicked()));
        count++;
    }

    /* Setup the request response controller */
    reqTimer = new QTimer();
    reqTimer->setInterval(200);

    connect(reqTimer, SIGNAL(timeout()), this, SLOT(requestSlot()));

}

void MainWindow::on_reqCheckBoxClicked(){
    QObject* obj = sender();

    qDebug() << ((QCheckBox*)obj)->text().remove('&');

    QJsonObject requests = initialization::getConfig("requests.json");
    QJsonArray items = requests.value(((QCheckBox*)obj)->text().remove('&')).toObject().value("items").toArray();
    //qDebug() << requests.value(((QCheckBox*)obj)->text().remove('&')).toObject().value("items").toArray();
    for(int i = 0; i < items.size(); i++){
        if(((QCheckBox*)obj)->checkState()){
            reqList.append(items[i].toString());
        }else{
            reqList.removeAll(items[i].toString());
        }
    }
}

void MainWindow::on_resCheckBoxClicked(){
    QObject* obj = sender();

    qDebug() << "res " + ((QCheckBox*)obj)->text() + ((QCheckBox*)obj)->checkState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Connect to server checkbox */
void MainWindow::on_connectToServer_clicked(bool checked)
{
    if(checked){
        socket->connectToHost(ui->ipTextEdit->toPlainText(), 2000);
    }else{
        socket->disconnectFromHost();
    }
}

void MainWindow::on_shouldRequest_clicked(bool checked)
{
    if(checked){
        reqTimer->start();
    }else{
        reqTimer->stop();
    }
}

void MainWindow::requestSlot(){

    QString request = "{\"type\":\"Request\",\"items\":[";
    for(int i = 0; i < reqList.size(); i++){
        request += reqList[i];
        if(i < reqList.size() -1){
            request+= ", ";
        }
    }
    request += "]}";
    qDebug() << request;

    char message[512];
    int len = CreateMessage(message, request.toLatin1().data(), TARGETLOCATOR, 0);

    QByteArray txMessage;
    txMessage.setRawData(message, len);

    int bytesSent = socket->send(txMessage);
    qDebug() << "bytesSent:" << bytesSent << "\n";

}
