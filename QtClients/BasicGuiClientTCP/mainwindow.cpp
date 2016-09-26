#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    tcpSocket = new ClientSocket();
    connect(this, SIGNAL(on_commStatButton_clicked()), tcpSocket, SLOT(commStatRequested()));
    connect(tcpSocket, SIGNAL(sendCommStatTargetLocator(QByteArray)), this, SLOT(on_targetLocatorTextEdit_textChanged(QByteArray)));
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(QString)), this, SLOT(on_connectedToServerTextEdit_textChanged(QString)));
    connect(tcpSocket, SIGNAL(sentCommStatSignal(bool)), this, SLOT(on_sentCommStatTextEdit_textChanged(bool)));
    //connect(this, SIGNAL(connectToHostUi(QString,int)), tcpSocket, SLOT(connectToHost(QString,int)));
    commStatRequestCounter = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// If pressed then get comm stats from PICs
void MainWindow::on_commStatButton_clicked()
{
    qDebug() << "Client connected event on button click";
}

// This determines whether or not to connect to the server
void MainWindow::on_connectToServerCheckBox_toggled(bool checked)
{
    // Connecting to the server if the check box is checked.
    // Disconnecting otherwise
    if(checked == true){
        QJsonObject config = initialization::getConfig();

        QString serverIp = config.value("serverIp").toString();
        int serverPort = config.value("serverPort").toInt();

        tcpSocket->connectToHost(serverIp,serverPort);
    }
    else{
        qDebug() << "Client disconnected";
    }
}

void MainWindow::on_connectedToServerTextEdit_textChanged(bool connectedToServerBool)
{
    ui->connectedToServerTextEdit->clear();
    ui->connectedToServerTextEdit->append(QString(connectedToServerBool));
}

void MainWindow::on_targetLocatorTextEdit_textChanged(QByteArray commStatTargetLocator)
{
    qDebug() << "Added to target locator text edit";
    ui->targetGrabberTextEdit->append(commStatTargetLocator);
}

void MainWindow::on_sentCommStatTextEdit_textChanged(bool sentCommStatBool)
{
    if( sentCommStatBool == true){
        commStatRequestCounter = commStatRequestCounter + 1;
    }
    else{
        commStatRequestCounter = commStatRequestCounter;
    }
    ui->sentCommStatTextEdit->clear();
    ui->sentCommStatTextEdit->append(QString(commStatRequestCounter));
}
