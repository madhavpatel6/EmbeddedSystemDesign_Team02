#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientsocket.h"
#include "initialization.h"

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
    void connectToHostUi(QString ip, int port);
    void on_commStatButton_clicked();
public slots:
    void on_targetLocatorTextEdit_textChanged(QByteArray commStatTargetLocator);
    void on_connectedToServerTextEdit_textChanged(bool connectedToServerBool);
    void on_sentCommStatTextEdit_textChanged(bool sentCommStatBool);
private slots:
    //void on_commStatButton_clicked();

    void on_connectToServerCheckBox_toggled(bool checked);

    //void on_connectedToServerTextEdit_textChanged();

    //void on_targetLocatorTextEdit_textChanged();

    //void on_sentCommStatTextEdit_textChanged();

private:
    Ui::MainWindow *ui;
    ClientSocket::QObject *tcpSocket;
    int commStatRequestCounter;
};

#endif // MAINWINDOW_H
