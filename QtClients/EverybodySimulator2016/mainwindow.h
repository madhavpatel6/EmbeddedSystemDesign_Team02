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

private slots:
    void on_checkBox_clicked(bool checked);
    void on_reqCheckBoxClicked();
    void on_resCheckBoxClicked();

private:
    Ui::MainWindow *ui;
    ClientSocket *socket;
};

#endif // MAINWINDOW_H
