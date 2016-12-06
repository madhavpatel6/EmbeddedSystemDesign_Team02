#include "mainwindow.h"
#include "initialization.h"
#include "clientsocket.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
