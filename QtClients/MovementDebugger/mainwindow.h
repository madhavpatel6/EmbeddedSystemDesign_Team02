#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientsocket.h"
#include "initialization.h"
#include "QTimer"
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *event);

signals:
    void pb_forwardClicked(void);
    void pb_backClicked(void);
    void pb_leftClicked(void);
    void pb_rightClicked(void);

public slots:
    void on_pb_connectToServer_clicked();
    void on_pb_requestPosition_clicked();
    void HostConnectionEvent(bool connected);
    void updateLocation(char source, QString x, QString y);
    void updateOrientation(char source, QString orientation);
    void updateLineLocation(int location);

private slots:
    void on_pb_forward_clicked();
    void on_pb_back_clicked();
    void on_pb_left_clicked();
    void on_pb_right_clicked();

    void on_pb_requestLineSensor_clicked();

private:
    Ui::MainWindow *ui;
    ClientSocket *tcpSocket;
    QTimer *requestTimer;
    QTimer *requestTimer2;
};

#endif // MAINWINDOW_H
