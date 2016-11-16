#ifndef PFWINDOW_H
#define PFWINDOW_H

#include <QMainWindow>
#include "clientsocket.h"
#include "QTimer"
#include "varianceform.h"
#include "req_respform.h"
#include "tg_req_respform.h"

namespace Ui {
class pfWindow;
}

class pfWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit pfWindow(QWidget *parent = 0);
    ~pfWindow();

signals:
    void varianceSignal(char source, QString alignmentData0,
                        QString alignmentData1, QString alignmentData2,
                        QString alignmentData3, QString alignmentData4);

public slots:
    void serverConnected(bool connected);
    void updateAlignmentData(char source, QString alignmentData0,
                             QString alignmentData1, QString alignmentData2,
                             QString alignmentData3, QString alignmentData4,
                             QString distanceFromTarget, QString angleToTarget,
                             QString alignmentBool_0, QString alignmentBool_1,
                             QString alignmentBool_2, QString alignmentBool_3,
                             QString alignmentBool_4, QString alignmentGrabberBool,
                             QString alignmentInfo);
    void updateAcquiredSlot(QString targetAcuiredBool);

private slots:
    void on_connectToServerButton_clicked();

    void on_disconnectFromServerButton_clicked();

    void on_startSensingButton_clicked();

    void on_proximitySlider_valueChanged(int value);

    void on_proximityPushButton_clicked();

    void sendProxSlot();

    void on_actionCalculate_Variance_triggered();

    void on_sendMovementPushButton_clicked();

    void on_sendOutsideofArenaPushButton_clicked();

    void on_actionRequests_Responses_triggered();

    void on_requestTargetAcquiredpushButton_clicked();

    void on_actionTG_Requests_and_Responses_triggered();

private:
    Ui::pfWindow *ui;
    ClientSocket *tcpSocket;
    QTimer *requestTimer;
    QTimer *sendProxTimer;
    varianceForm *varianceFormObj;
    req_respForm *req_respFormObj;
    tg_req_respForm *tg_req_respFormObj;
    int IR0RequestCounter;
    int IR1RequestCounter;
    int IR2RequestCounter;
    int IR3RequestCounter;
    int IR4RequestCounter;
    int proximitySliderValue;
    bool sendMovementClickedBool;
    bool sendOutsideArenaClickedBool;
    bool requestTargetAcquiredBool;
};

#endif // PFWINDOW_H
