#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    config = new initialization();


    socket = new ClientSocket();

    QJsonObject requests = config->getConfig("requests.json");

    QJsonObject responses = config->getConfig("responses.json");


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

    /* Setup the request timer */
    reqTimer = new QTimer();
    reqTimer->setInterval(200);

    connect(reqTimer, SIGNAL(timeout()), this, SLOT(requestSlot()));

    /* Setup response handler */
    connect(socket, SIGNAL(dataRead(QByteArray)), this, SLOT(dataReadSlot(QByteArray)));

    /* print what module we are emulating to the gui */
    ui->myModuleLabel->setText(QString(MYMODULE));

    qDebug() << config->getConfig("responses.json")["targetProximity"].toString();
    QJsonObject object
    {
        {"targetProximity", "300"}
    };
    config->changeResponse(object);
    qDebug() << config->getConfig("responses.json")["targetProximity"].toString();

}

void MainWindow::on_reqCheckBoxClicked(){
    QObject* obj = sender();

    qDebug() << ((QCheckBox*)obj)->text().remove('&');

    QJsonObject requests = config->getConfig("requests.json");
    // QJsonArray items = requests.value(((QCheckBox*)obj)->text().remove('&')).toObject().value("items").toArray();
    //qDebug() << requests.value(((QCheckBox*)obj)->text().remove('&')).toObject().value("items").toArray();

    if(((QCheckBox*)obj)->checkState()){
        reqObjList.append(requests.value(((QCheckBox*)obj)->text().remove('&')).toObject());
    }else{
        reqObjList.removeAll(requests.value(((QCheckBox*)obj)->text().remove('&')).toObject());
    }
}

void MainWindow::on_resCheckBoxClicked(){
    QObject* obj = sender();
    if(((QCheckBox*)obj)->checkState()){
        resEnabled.append((((QCheckBox*)obj)->text().remove('&')));
    }else{
        resEnabled.removeAll((((QCheckBox*)obj)->text().remove('&')));
    }
    //qDebug() << "res " + ((QCheckBox*)obj)->text() + ((QCheckBox*)obj)->checkState();
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

    QString request;
    for(int i = 0; i < reqObjList.size(); i++){
        request = "{\"type\":\"Request\",\"items\":[";
        for(int j = 0; j < reqObjList[i].value("items").toArray().size(); j++){
            request += '\"' + reqObjList[i].value("items").toArray()[j].toString() + '\"';
            if(j < reqObjList[i].value("items").toArray().size()-1){
                request+= ", ";
            }
        }
        request += "]}";
        char message[512];
        char destination = reqObjList[i].value("destination").toString().toLatin1()[0];

        int len = CreateMessage(message, request.toLatin1().data(), destination, 0);

        QByteArray txMessage;
        txMessage.setRawData(message, len);

        int bytesSent = socket->send(txMessage);

        qDebug() << "bytesSent:" << bytesSent << "\n";

    }


}
void MainWindow::dataReadSlot(QByteArray data){

    QByteArray array = data;
    char buffer[MAXMESSAGESIZE];
    char source, messageCount;
    int numOfErrors = 0;
    bool isError;
    for(int i = 0; i < array.size(); i++) {
        bool isCompleted = ParseMessage(array[i], buffer, &source, &messageCount, &isError);
        if(isCompleted) {
            QJsonDocument doc(QJsonDocument::fromJson(buffer));
            QJsonObject json = doc.object();
            QString type = json["type"].toString();
            if(type == QStringLiteral("Response")) {
                qDebug() << "Response: " << json;
            }
            else if(type == QStringLiteral("Request")){
                qDebug() << "Request: " << json["items"];

                QJsonObject responses = config->getConfig("responses.json");

                QString jsonMessage = "{\"type\": \"Response\",";
                for(int i = 0; i < json["items"].toArray().size(); i++){
                    if(resEnabled.contains(json["items"].toArray()[i].toString())){
                        QString key = "\"" + json["items"].toArray()[i].toString() + "\"";
                        QString value;
                        if(responses.value(json["items"].toArray()[i].toString()).isObject()){
                            value = QJsonDocument(responses.value(json["items"].toArray()[i].toString()).toObject()).toJson();
                        }else{
                            value = "\"" + responses.value(json["items"].toArray()[i].toString()).toString() + "\"";
                        }
                        jsonMessage += key + ":" + value + ",";
                    }
                }
                jsonMessage.chop(1); // this removes the extra comma at the end
                jsonMessage += "}";
                // qDebug() << jsonMessage;
                char message[512];
                char destination = source;

                int len = CreateMessage(message, jsonMessage.toLatin1().data(), destination, 0);

                QByteArray txMessage;
                txMessage.setRawData(message, len);

                int bytesSent = socket->send(txMessage);

                qDebug() << "bytesSent:" << bytesSent << "\n";

            }else if(type == QStringLiteral("PID")){
                // qDebug() << "PID Message";
                if(tuner){
                    emit pidTunerMessage(json);
                }
            }
        }
        else {
            numOfErrors++;
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    switch(MYMODULE){
        case PATHFINDER:
            pSim = new PathSimulator();
            pSim->setConfig(config);
            pSim->show();
        break;
        default:
        break;
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    tuner = new pidTuner();
    tuner->show();

    connect(this, SIGNAL(pidTunerMessage(QJsonObject)), tuner, SLOT(motorUpdate(QJsonObject)));
}
