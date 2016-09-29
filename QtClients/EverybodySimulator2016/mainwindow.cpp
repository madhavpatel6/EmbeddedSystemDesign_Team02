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

        connect (reqBox[count], SIGNAL(clicked()), this, SLOT(on_resCheckBoxClicked()));
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
}

void MainWindow::on_reqCheckBoxClicked(){
    QObject* obj = sender();

    qDebug() << "req " + ((QCheckBox*)obj)->text() + ((QCheckBox*)obj)->checkState();
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
void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked){
        socket->connectToHost(ui->ipTextEdit->toPlainText(), 2000);
    }else{
        socket->disconnectFromHost();
    }
}
