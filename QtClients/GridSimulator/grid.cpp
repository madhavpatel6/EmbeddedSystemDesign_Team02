#include "grid.h"
#include <QDebug>
#include <QPainter>
namespace Grid {

//void draw(QPainter *painter) {
//    int colorVal = (-1.0*count/MAXIMUM*127 + 127);
//    painter->fillRect(box, QBrush(QColor(colorVal, colorVal, colorVal)));
//}

bool checkBounds(int x, int y) {
    return y < HEIGHT && x < WIDTH && y >= 0 && x >= 0;
}

void initializeGrid(GridType grid) {
    for(int locY = 0; locY < HEIGHT; locY++) {
        for(int locX = 0; locX < WIDTH; locX++) {
            grid[locY][locX] = 0;
        }
    }
}

void incrementIndex(int x, int y, GridType grid) {
//    qDebug() << "Max (x,y) = (" << x << ", " << y <<")";
    if(grid[y][x] < (MAXIMUM - 15)) {
        grid[y][x]+=15;
    }
}

void decrementIndex(int x, int y, GridType grid) {
//    qDebug() << "(x,y) = (" << x << ", " << y <<")";
    if(grid[y][x] > MINIMUM + 5) {
        grid[y][x]-=5;
    }
}

void draw(QPainter* painter, GridType grid, size_t size) {
    for(int locY = 0; locY < HEIGHT; locY++) {
        for(int locX = 0; locX < WIDTH; locX++) {
            QRect box = QRect(QPoint(locX*size,(locY)*size), QSize(size,size));
            int colorVal = (-1.0*grid[locY][locX]/MAXIMUM*127 + 127);
            painter->fillRect(box, QBrush(QColor(colorVal, colorVal, colorVal)));
//            painter->drawRect(box);
        }
    }
}
}