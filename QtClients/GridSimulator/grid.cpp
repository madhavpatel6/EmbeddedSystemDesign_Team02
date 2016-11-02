#include "grid.h"
#include <QDebug>
#include <QPainter>
namespace Grid {

bool checkBounds(int x, int y) {
    return y < HEIGHT && x < WIDTH && y >= 0 && x >= 0;
}

void initializeGrid(GridType grid) {
    for(int locY = 0; locY < HEIGHT; locY++) {
        for(int locX = 0; locX < WIDTH; locX++) {
            grid[locY][locX] = MAXIMUM;
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
#include <QImage>
void draw(QPainter* painter, GridType grid, size_t size) {
    QImage image(WIDTH*size, HEIGHT*size, QImage::Format_RGB32);
//    QPainter savePainter;
//    savePainter.begin(&image);
//    savePainter.translate(0,HEIGHT*size);
//    savePainter.scale(1,-1);
    for(int locY = 0; locY < HEIGHT; locY++) {
        for(int locX = 0; locX < WIDTH; locX++) {
            QRect box = QRect(QPoint(locX*size,(locY)*size), QSize(size,size));
            int colorVal = (-1.0*grid[locY][locX]/MAXIMUM*127 + 127);
            painter->fillRect(box, QBrush(QColor(colorVal, colorVal, colorVal)));
//            savePainter.fillRect(box, QBrush(QColor(colorVal, colorVal, colorVal)));
//            painter->drawRect(box);
        }
    }
//    image.save("image.jpg");
}
}
