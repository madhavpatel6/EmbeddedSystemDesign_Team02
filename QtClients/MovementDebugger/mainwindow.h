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

signals:
    void connectToServerButtonClicked(QString ip, QString port);

public slots:
    void on_btn_connectToServer_clicked();
    void on_btn_requestPosition_clicked();
    void HostConnectionEvent(bool connected);
    void positionRequestSent();
    void updateLocation(char source, QString x, QString y);
    void updateOrientation(char source, QString orientation);

private:
    Ui::MainWindow *ui;
    ClientSocket *tcpSocket;
    QTimer *requestTimer;
    int requestCounter;
};

#endif // MAINWINDOW_H
