#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsView>
#include <QGridLayout>
#include "gridscene.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow win;
    win.show();
    return a.exec();
}
