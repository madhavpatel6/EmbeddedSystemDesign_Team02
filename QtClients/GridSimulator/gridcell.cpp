#include "gridcell.h"
#include <QDebug>
GridCell::GridCell(int _x, int _y, int size) {
    box = QRect(QPoint(_x*size,(_y)*size), QSize(size,size));
//    box.setBottomLeft(QPoint(_x*size,_y*size));
//        qDebug() << "Created grid cell " << box.bottomLeft();
    count = 0;
}

void GridCell::draw(QPainter *painter) {
    int colorVal = (1.0*count/MINIMUM*127 + 127);
    painter->fillRect(box, QBrush(QColor(colorVal, colorVal, colorVal)));
//    painter->drawRect(box);
}

void GridCell::decrement() {
    if(count != MINIMUM)
        count--;
}

void GridCell::increment() {
    if(count != MAXIMUM) {
        count++;
//        qDebug() << "Cell count " << count;
    }
}
