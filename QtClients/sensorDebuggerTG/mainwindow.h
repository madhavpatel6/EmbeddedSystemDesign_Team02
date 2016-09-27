#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientsocket.h"
#include "initialization.h"
#include "QTimer"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void serverConnected(bool connected);
    void updateAlignmentData(char source, QString alignmentData);

private slots:
    void on_connectToServerButton_clicked();

    void on_disconnectFromServerButton_clicked();

    void on_startSensingButton_clicked();

private:
    Ui::MainWindow *ui;
    ClientSocket *tcpSocket;
    QTimer *requestTimer;
    int sensorRequestCounter;
};

#endif // MAINWINDOW_H
