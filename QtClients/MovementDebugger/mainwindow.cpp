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
    connect(this, SIGNAL(sendInitialData(QString,QString,QString,QString)), tcpSocket, SLOT(sendInitialData(QString,QString,QString,QString)));

    QStringList colHeaders;
    colHeaders << "x" << "y";
    ui->tbl_vertices->setHorizontalHeaderLabels(colHeaders);
    ui->tbl_vertices->setRowCount(ui->sb_numVertices->value());
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
    QString mode;
    QString position;
    QString numVertices;
    QString vertices;
    int count = 0;

    mode = "\"mode\":";

    if (ui->rb_debug->isChecked()) {
        mode.append("\"D\"");
    } else if (ui->rb_lawnmower->isChecked()) {
        mode.append("\"L\"");
    } else if (ui->rb_random->isChecked()) {
        mode.append("\"R\"");
    }

    position = "\"x\":\"" + QString::number(ui->sb_x_initial->value()) + "\"," +
            "\"y\":\"" + QString::number(ui->sb_y_initial->value()) + "\"," +
            "\"orientation\":\"" + QString::number(ui->sb_orientation_initial->value()) + "\"";

    numVertices = "\"numVertices\":\"" + QString::number(ui->sb_numVertices->value()) + "\"";

    vertices = "\"vertices\":[";

    for (int i = 0; i < ui->sb_numVertices->value(); i++) {
        if (ui->tbl_vertices->item(i, 0) && ui->tbl_vertices->item(i, 1)) {
            count++;
            vertices.append("[\"" + ui->tbl_vertices->item(i, 0)->text() + "\",\"" +
                            ui->tbl_vertices->item(i, 1)->text() + "\"]");

            if (i != ui->sb_numVertices->value() - 1) {
                vertices.append(",");
            }
        }
    }

    vertices.append("]");

    if (count == ui->sb_numVertices->value()) {
        emit sendInitialData(mode, position, numVertices, vertices);
    }
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
    ui->tbl_vertices->setRowCount(ui->sb_numVertices->value());

    QString mode;
    QString position;
    QString numVertices;
    QString vertices;
    int count = 0;

    mode = "\"mode\":";

    if (ui->rb_debug->isChecked()) {
        mode.append("\"D\"");
    } else if (ui->rb_lawnmower->isChecked()) {
        mode.append("\"L\"");
    } else if (ui->rb_random->isChecked()) {
        mode.append("\"R\"");
    }

    position = "\"x\":\"" + QString::number(ui->sb_x_initial->value()) + "\"," +
            "\"y\":\"" + QString::number(ui->sb_y_initial->value()) + "\"," +
            "\"orientation\":\"" + QString::number(ui->sb_orientation_initial->value()) + "\"";

    numVertices = "\"numVertices\":\"" + QString::number(ui->sb_numVertices->value()) + "\"";

    vertices = "\"vertices\":[";

    for (int i = 0; i < ui->sb_numVertices->value(); i++) {
        if (ui->tbl_vertices->item(i, 0) && ui->tbl_vertices->item(i, 1)) {
            count++;
            vertices.append("[\"" + ui->tbl_vertices->item(i, 0)->text() + "\",\"" +
                            ui->tbl_vertices->item(i, 1)->text() + "\"]");

            if (i != ui->sb_numVertices->value() - 1) {
                vertices.append(",");
            }
        }
    }

    vertices.append("]");

    QString response_begin = "{\"type\":\"Response\",\"InitialData\":{";
    QString response_end = "}}";
    qDebug() << response_begin + mode + "," + position + "," + numVertices + "," + vertices + response_end;
}
