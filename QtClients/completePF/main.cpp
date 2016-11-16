#include "pfwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pfWindow w;
    w.show();

    return a.exec();
}
