#include "gridcell.h"
#include <QDebug>
GridCell::GridCell(int _x, int _y, int size) {
    box = QRect(_x,_y,size,size);
    count = 0;
//    state = UNKNOWN;
}

void GridCell::draw(QPainter *painter) {
    int colorVal = (1.0*count/MINIMUM*127 + 127);
    painter->fillRect(box, QBrush(QColor(colorVal, colorVal, colorVal)));
    painter->drawRect(box);
}

void GridCell::decrement() {
    if(count != MINIMUM)
        count--;
}

void GridCell::increment() {
    if(count != MINIMUM)
        count++;
}
