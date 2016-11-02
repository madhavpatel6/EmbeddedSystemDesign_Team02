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
    void pb_forwardClicked(int distance);
    void pb_backClicked(int distance);
    void pb_leftClicked(int degrees);
    void pb_rightClicked(int degrees);

public slots:
    void on_pb_connectToServer_clicked();
    void HostConnectionEvent(bool connected);
    void updateMovement(char source, QString x, QString y, QString orientation,
                        QString action, QString amount);
    void updateLineLocation(int location);

private slots:
    void on_pb_forward_clicked();
    void on_pb_back_clicked();
    void on_pb_left_clicked();
    void on_pb_right_clicked();
    void on_pb_requestMovement_clicked();
    void on_pb_requestLineSensor_clicked();
    void on_sl_distance_valueChanged(int value);
    void on_sl_degrees_valueChanged(int value);
    void on_sb_distance_valueChanged(int arg1);
    void on_sb_degrees_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    ClientSocket *tcpSocket;
    QTimer *requestTimer;
    QTimer *requestTimer2;
};

#endif // MAINWINDOW_H
