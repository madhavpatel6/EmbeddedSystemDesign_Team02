#ifndef TARGETALIGNMENT_H
#define TARGETALIGNMENT_H

#include <QMainWindow>
#include "clientsocket.h"
#include "initialization.h"
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
    void updateAlignmentData(char source, QString alignmentData0, QString alignmentData1, QString alignmentData2);

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
