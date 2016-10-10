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
    void pb_forwardClicked(void);
    void pb_backClicked(void);
    void pb_leftClicked(void);
    void pb_rightClicked(void);

public slots:
    void on_btn_connectToServer_clicked();
    void on_btn_requestPosition_clicked();
    void HostConnectionEvent(bool connected);
    void positionRequestSent();
    void updateLocation(char source, QString x, QString y);
    void updateOrientation(char source, QString orientation);

private slots:
    void on_pb_forward_clicked();
    void on_pb_back_clicked();
    void on_pb_left_clicked();
    void on_pb_right_clicked();

private:
    Ui::MainWindow *ui;
    ClientSocket *tcpSocket;
    QTimer *requestTimer;
    int requestCounter;
};

#endif // MAINWINDOW_H
