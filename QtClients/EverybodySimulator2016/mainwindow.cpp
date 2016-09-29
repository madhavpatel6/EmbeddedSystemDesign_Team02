#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    socket = new ClientSocket();

    QJsonObject reqres = initialization::getConfig();


    QCheckBox* reqBox[reqres.size()];
    QCheckBox* resBox[reqres.size()];
    int count = 0;
    for(auto i = reqres.begin(); i != reqres.end(); i++){
        reqBox[count] = new QCheckBox();
        reqBox[count]->setText(i.key());
        ui->requestsEnable->addWidget(reqBox[count]);
        connect (reqBox[count], SIGNAL(clicked()), this, SLOT(on_resCheckBoxClicked()));

        resBox[count] = new QCheckBox();
        resBox[count]->setText(i.key());
        ui->responsesEnable->addWidget(resBox[count]);

        connect (resBox[count], SIGNAL(clicked()), this, SLOT(on_resCheckBoxClicked()));
        count++;
    }
}
void MainWindow::on_reqCheckBoxClicked(){
    QObject* obj = sender();

    qDebug() << "req" + ((QCheckBox*)obj)->text() + ((QCheckBox*)obj)->checkState();
}
void MainWindow::on_resCheckBoxClicked(){
    QObject* obj = sender();

    qDebug() << "res" + ((QCheckBox*)obj)->text() + ((QCheckBox*)obj)->checkState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked){
        socket->connectToHost(ui->ipTextEdit->toPlainText(), 2000);
    }else{
        socket->disconnectFromHost();
    }
}
