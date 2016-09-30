#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QJsonArray>
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

private slots:
    void on_connectToServer_clicked(bool checked);
    void on_reqCheckBoxClicked();
    void on_resCheckBoxClicked();

    void on_shouldRequest_clicked(bool checked);
    void requestSlot();

private:
    Ui::MainWindow *ui;
    ClientSocket *socket;
    QTimer *reqTimer;
    QStringList reqList;

};

#endif // MAINWINDOW_H
