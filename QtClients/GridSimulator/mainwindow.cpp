#include "mainwindow.h"
#include <QGraphicsView>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    grid = new GridScene(this);
}

MainWindow::~MainWindow()
{

}
