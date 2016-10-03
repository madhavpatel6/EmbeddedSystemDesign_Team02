#ifndef TARGETALIGNMENT_H
#define TARGETALIGNMENT_H

#include <QMainWindow>
#include "clientsocket.h"
#include "QTimer"

namespace Ui {
class targetAlignment;
}

class targetAlignment : public QMainWindow
{
    Q_OBJECT

public:
    explicit targetAlignment(QWidget *parent = 0);
    ~targetAlignment();
public slots:
    void serverConnected(bool connected);
    void updateAlignmentData(char source, QString alignmentData0, QString alignmentData1, QString alignmentData2, QString alignmentBool_0, QString alignmentBool_1, QString alignmentBool_2, QString alignmentGrabberBool);

private slots:
    void on_connectToServerButton_clicked();

    void on_disconnectFromServerButton_clicked();

    void on_startSensingButton_clicked();

private:
    Ui::targetAlignment *ui;
    ClientSocket *tcpSocket;
    QTimer *requestTimer;
    int IR0RequestCounter;
    int IR1RequestCounter;
    int IR2RequestCounter;
};

#endif // TARGETALIGNMENT_H
