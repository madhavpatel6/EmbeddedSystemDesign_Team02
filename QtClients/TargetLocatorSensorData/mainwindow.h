#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
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
    void receiveUpdate(QString leftFTSensor, QString rightFTSensor, QString leftFBSensor, QString rightFBSensor, QString leftUltra, QString middleUltra, QString rightUltra, QString leftSideUltra, QString rightSideUltra);
    void receiveTime(QString time);

    void on_pushButton_clicked();

private:
    float computeMean(QVector<float> vec);
    float computeStd(QVector<float> vec);
    void updateStatistics(QVector<float> vec, QLabel* label);
    QVector<float> valuesTL;
    QVector<float> valuesTR;
    QVector<float> valuesBL;
    QVector<float> valuesBR;
    QVector<float> valuesUL;
    QVector<float> valuesUM;
    QVector<float> valuesUR;
    QVector<float> valuesLS;
    QVector<float> valuesRS;
    Ui::MainWindow *ui;
    QTimer* requestTimer;
    ClientSocket* tcpSocket;
};

#endif // MAINWINDOW_H
