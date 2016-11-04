#include "mainwindow.h"
#include "ui_mainwindow.h"

#define REQUESTRATE_MS 200

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    requestTimer = new QTimer();
    requestTimer2 = new QTimer();
    requestTimer->setInterval(REQUESTRATE_MS);
    requestTimer2->setInterval(REQUESTRATE_MS);

    ui->setupUi(this);
    tcpSocket = new ClientSocket();

    connect(requestTimer, SIGNAL(timeout()), tcpSocket, SLOT(movementRequested()));
    connect(requestTimer2, SIGNAL(timeout()), tcpSocket, SLOT(lineLocationRequested()));
    connect(this, SIGNAL(pb_forwardClicked(int)), tcpSocket, SLOT(sendForwardCommand(int)));
    connect(this, SIGNAL(pb_backClicked(int)), tcpSocket, SLOT(sendBackCommand(int)));
    connect(this, SIGNAL(pb_leftClicked(int)), tcpSocket, SLOT(sendLeftCommand(int)));
    connect(this, SIGNAL(pb_rightClicked(int)), tcpSocket, SLOT(sendRightCommand(int)));
    connect(this, SIGNAL(pb_clearClicked(bool)), tcpSocket, SLOT(sendClear(bool)));
    connect(this, SIGNAL(pb_obstacleClicked(bool)), tcpSocket, SLOT(sendObstacle(bool)));
    connect(tcpSocket, SIGNAL(serverIsConnectedSignal(bool)), this, SLOT(HostConnectionEvent(bool)));
    connect(tcpSocket, SIGNAL(sendMovement(char,QString,QString,QString,QString,QString)),
            this, SLOT(updateMovement(char,QString,QString,QString,QString,QString)));
    connect(tcpSocket, SIGNAL(sendLineLocation(int)), this, SLOT(updateLineLocation(int)));
    connect(tcpSocket, SIGNAL(initialRequest()), this, SLOT(sendInitialResponse()));
    connect(this, SIGNAL(sendInitialData(char)), tcpSocket, SLOT(sendInitialData(char)));

//    ui->lbl_1->setVisible(false);   ui->le_x1->setVisible(false);   ui->le_y1->setVisible(false);
//    ui->lbl_2->setVisible(false);   ui->le_x2->setVisible(false);   ui->le_y2->setVisible(false);
//    ui->lbl_3->setVisible(false);   ui->le_x3->setVisible(false);   ui->le_y3->setVisible(false);
//    ui->lbl_4->setVisible(false);   ui->le_x4->setVisible(false);   ui->le_y4->setVisible(false);
//    ui->lbl_5->setVisible(false);   ui->le_x5->setVisible(false);   ui->le_y5->setVisible(false);
//    ui->lbl_6->setVisible(false);   ui->le_x6->setVisible(false);   ui->le_y6->setVisible(false);
//    ui->lbl_7->setVisible(false);   ui->le_x7->setVisible(false);   ui->le_y7->setVisible(false);
//    ui->lbl_8->setVisible(false);   ui->le_x8->setVisible(false);   ui->le_y8->setVisible(false);
//    ui->lbl_9->setVisible(false);   ui->le_x9->setVisible(false);   ui->le_y9->setVisible(false);
//    ui->lbl_10->setVisible(false);   ui->le_x10->setVisible(false);   ui->le_y10->setVisible(false);
//    ui->lbl_11->setVisible(false);   ui->le_x11->setVisible(false);   ui->le_y11->setVisible(false);
//    ui->lbl_12->setVisible(false);   ui->le_x12->setVisible(false);   ui->le_y12->setVisible(false);
//    ui->lbl_13->setVisible(false);   ui->le_x13->setVisible(false);   ui->le_y13->setVisible(false);
//    ui->lbl_14->setVisible(false);   ui->le_x14->setVisible(false);   ui->le_y14->setVisible(false);
//    ui->lbl_15->setVisible(false);   ui->le_x15->setVisible(false);   ui->le_y15->setVisible(false);
//    ui->lbl_16->setVisible(false);   ui->le_x16->setVisible(false);   ui->le_y16->setVisible(false);
//    ui->lbl_17->setVisible(false);   ui->le_x17->setVisible(false);   ui->le_y17->setVisible(false);
//    ui->lbl_18->setVisible(false);   ui->le_x18->setVisible(false);   ui->le_y18->setVisible(false);
//    ui->lbl_19->setVisible(false);   ui->le_x19->setVisible(false);   ui->le_y19->setVisible(false);
//    ui->lbl_20->setVisible(false);   ui->le_x20->setVisible(false);   ui->le_y20->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete requestTimer;
    delete tcpSocket;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A) {
        tcpSocket->sendLeftCommand(ui->sl_degrees->value());
    } else if (event->key() == Qt::Key_D) {
        tcpSocket->sendRightCommand(ui->sl_degrees->value());
    } else if (event->key() == Qt::Key_W) {
        tcpSocket->sendForwardCommand(ui->sl_distance->value());
    } else if (event->key() == Qt::Key_S) {
        tcpSocket->sendBackCommand(ui->sl_distance->value());
    }
}

void MainWindow::on_pb_connectToServer_clicked()
{
    tcpSocket->connectToHost(ui->le_IPAddress->text(), ui->le_port->text().toInt());
}

void MainWindow::on_pb_requestMovement_clicked()
{
    if(requestTimer->isActive()) {
        ui->pb_requestMovement->setText(QString("Request"));
        requestTimer->stop();
    }
    else {
        ui->pb_requestMovement->setText(QString("Stop"));
        requestTimer->start();
    }
}

void MainWindow::HostConnectionEvent(bool connected) {
    qDebug() << "Host Connection Event " << connected;
    if(connected) {
        ui->pb_connectToServer->setText(QString("Disconnect From Server"));
    }
    else {
        ui->pb_connectToServer->setText(QString("Connect To Server"));
    }
}

void MainWindow::updateMovement(char source, QString x, QString y, QString orientation,
                                QString action, QString amount)
{
    int temp = (int)orientation.toFloat();
    temp = temp % 360;

    switch(source) {
        case SEARCHERMOVER: {
            ui->lbl_x_positionVal->setText(x);
            ui->lbl_y_positionVal->setText(y);
            ui->lbl_orientationVal->setText(QString::number(temp));
            ui->lbl_actionVal->setText(action);
            ui->lbl_amountVal->setText(amount);
            break;
        }
        default: {
            break;
        }
    }
}

void MainWindow::updateLineLocation(int location)
{
    ui->rb_lineSensor0->setChecked(location & 1);
    ui->rb_lineSensor1->setChecked(location & (1 << 1));
    ui->rb_lineSensor2->setChecked(location & (1 << 2));
    ui->rb_lineSensor3->setChecked(location & (1 << 3));
    ui->rb_lineSensor4->setChecked(location & (1 << 4));
    ui->rb_lineSensor5->setChecked(location & (1 << 5));
    ui->rb_lineSensor6->setChecked(location & (1 << 6));
    ui->rb_lineSensor7->setChecked(location & (1 << 7));
}

void MainWindow::sendInitialResponse()
{
    char mode;

    if (ui->rb_debug->isChecked()) {
        mode = 'D';
    } else if (ui->rb_lawnmower->isChecked()) {
        mode = 'L';
    } else if (ui->rb_random->isChecked()) {
        mode = 'R';
    }

    emit sendInitialData(mode);
}

void MainWindow::on_pb_forward_clicked()
{
    emit pb_forwardClicked(ui->sl_distance->value());
}

void MainWindow::on_pb_back_clicked()
{
    emit pb_backClicked(ui->sl_distance->value());
}

void MainWindow::on_pb_left_clicked()
{
    emit pb_leftClicked(ui->sl_degrees->value());
}

void MainWindow::on_pb_right_clicked()
{
    emit pb_rightClicked(ui->sl_degrees->value());
}

void MainWindow::on_pb_requestLineSensor_clicked()
{
    if(requestTimer2->isActive()) {
        ui->pb_requestLineSensor->setText(QString("Request"));
        requestTimer2->stop();
    }
    else {
        ui->pb_requestLineSensor->setText(QString("Stop"));
        requestTimer2->start();
    }
}

void MainWindow::on_sl_distance_valueChanged(int value)
{
    ui->sb_distance->setValue(value);
}

void MainWindow::on_sl_degrees_valueChanged(int value)
{
    ui->sb_degrees->setValue(value);
}

void MainWindow::on_sb_distance_valueChanged(int value)
{
    ui->sl_distance->setValue(value);
}

void MainWindow::on_sb_degrees_valueChanged(int value)
{
    ui->sl_degrees->setValue(value);
}

void MainWindow::on_pb_clear_clicked()
{
    ui->pb_obstacle->setChecked(false);

    if (ui->pb_clear->isChecked()) {
        emit pb_clearClicked(true);
    } else {
        emit pb_clearClicked(false);
    }
}

void MainWindow::on_pb_obstacle_clicked()
{
    ui->pb_clear->setChecked(false);

    if (ui->pb_obstacle->isChecked()) {
        emit pb_obstacleClicked(true);
    } else {
        emit pb_obstacleClicked(false);
    }
}

void MainWindow::on_sb_numVertices_valueChanged(int value)
{

}
