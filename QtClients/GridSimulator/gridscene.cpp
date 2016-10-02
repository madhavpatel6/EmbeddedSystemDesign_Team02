#include "gridscene.h"

GridScene::GridScene(QWidget *parent) : QWidget(parent)
{
    this->resize(WIDTH*CELL_SIZE + 100, HEIGHT*CELL_SIZE + 100);
    int locX = 0;
    int locY = 0;
    for(int i = 0; i < HEIGHT; i++) {
        std::vector<GridCell> tempV;
        for(int j = 0; j < WIDTH; j++) {
            GridCell tempCell = GridCell(locX,locY,CELL_SIZE);
            locX += CELL_SIZE;
            tempV.push_back(tempCell);
        }
        locX = 0;
        grid.push_back(tempV);
        locY += CELL_SIZE;
    }
}

void GridScene::paintEvent(QPaintEvent *) {
    QPainter painter;
    painter.begin(this);
    QPen pen(Qt::white,1, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
    qDebug() << "H " << this->size().height();
    qDebug() << "W " << this->size().width();
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            grid[i][j].draw(&painter);
        }
    }
    painter.end();
}

GridScene::~GridScene() {

}
