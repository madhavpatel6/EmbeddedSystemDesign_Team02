#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "clientsocket.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_requestButton_clicked();
    void on_connectToServer_clicked();
    void HostConnectionEvent(bool connected);
    void receiveUpdate(QString leftFTSensor, QString middleFTSensor, QString rightFTSensor, QString leftFBSensor, QString middleFBSensor, QString rightFBSensor, QString leftSDSensor, QString rightSDSensor);
private:
    Ui::MainWindow *ui;
    QTimer* requestTimer;
    ClientSocket* tcpSocket;
};

#endif // MAINWINDOW_H
