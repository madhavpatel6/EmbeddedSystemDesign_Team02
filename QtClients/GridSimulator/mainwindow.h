#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include "gridscene.h"
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GridScene *grid;
    QVBoxLayout* verticalLayout;
private:
    void setupUi(QWidget* mainwindow);
};

#endif // MAINWINDOW_H
