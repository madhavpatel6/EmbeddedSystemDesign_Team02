#ifndef PROXIMITYWINDOW_H
#define PROXIMITYWINDOW_H

#include <QMainWindow>
#include "clientsocket.h"
#include "QTimer"

namespace Ui {
class proximityWindow;
}

class proximityWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit proximityWindow(QWidget *parent = 0);
    ~proximityWindow();

public slots:
    void serverConnected(bool connected);
    void updateAlignmentData(char source, QString alignmentData0, QString alignmentData1, QString alignmentData2, QString alignmentBool_0, QString alignmentBool_1, QString alignmentBool_2, QString alignmentGrabberBool, QString alignmentInfo);

private slots:
    void on_connectToServerButton_clicked();

    void on_disconnectFromServerButton_clicked();

    void on_startSensingButton_clicked();

    void on_proximitySlider_valueChanged(int value);

    void on_proximityPushButton_clicked();

    void sendProxSlot();

private:
    Ui::proximityWindow *ui;
    ClientSocket *tcpSocket;
    QTimer *requestTimer;
    QTimer *sendProxTimer;
    int IR0RequestCounter;
    int IR1RequestCounter;
    int IR2RequestCounter;
    int proximitySliderValue;
};

#endif // PROXIMITYWINDOW_H
