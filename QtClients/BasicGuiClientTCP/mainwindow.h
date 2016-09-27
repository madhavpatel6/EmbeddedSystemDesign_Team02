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
   // void commStatPushed(bool commStatButtonBool);
    void connectToServerButtonClicked(QString ip, QString port);

public slots:
    void UpdateErrorCount(int count);
    void HostConnectionEvent(bool connected);
    void on_commStatsButton_clicked();
    void on_connectToServer_clicked();
    void CommStatsRequestSent();
    void UpdateCommStats(char source, QString goodMsg, QString commError, QString reqRecv, QString resRecv, QString reqSent, QString resSent);
private:
    Ui::MainWindow *ui;
    ClientSocket *tcpSocket;
    QTimer *requestTimer;
    int commStatRequestCounter;
};

#endif // MAINWINDOW_H
