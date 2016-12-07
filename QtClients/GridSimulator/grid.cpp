#include "grid.h"
#include <QDebug>
#include <QPainter>
#include "polygon.h"
namespace Grid {
VertexInformationType vertices;
void initializeVertices() {
    vertices.number = 7;
    vertices.vertices[0] = {10, 15};
    vertices.vertices[1] = {20, 10};
    vertices.vertices[2] = {60, 13};
    vertices.vertices[3] = {65, 45};
    vertices.vertices[4] = {45, 55};
    vertices.vertices[5] = {40, 42};
    vertices.vertices[6] = {20, 40};
}

bool isWithinArena(int x, int y) {
    point_t p;
    p.x = x;
    p.y = y;
    return PointInPolygon(p, vertices.number, vertices.vertices);
}

bool checkBounds(int x, int y) {
    return y < HEIGHT && x < WIDTH && y >= 0 && x >= 0;
}

void initializeGrid(GridType grid) {
    for(int locY = 0; locY < HEIGHT; locY++) {
        for(int locX = 0; locX < WIDTH; locX++) {
            grid[locY][locX] = MINIMUM;
        }
    }
}

void incrementIndex(int x, int y, GridType grid) {
//    qDebug() << "Max (x,y) = (" << x << ", " << y <<")";
    if(!checkBounds(x,y))
        return;
    if(grid[y][x] < (MAXIMUM - 15)) {
        grid[y][x]+=15;
    }
}

void decrementIndex(int x, int y, GridType grid) {
//    qDebug() << "(x,y) = (" << x << ", " << y <<")";
    if(!checkBounds(x,y))
        return;
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
