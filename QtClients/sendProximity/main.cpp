#include "proximitywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    proximityWindow w;
    w.show();

    return a.exec();
}
