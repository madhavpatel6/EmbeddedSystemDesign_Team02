#ifndef GRIDSCENE_H
#define GRIDSCENE_H
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>
#include <QWidget>
#include "gridcell.h"
#include <vector>
class GridScene : public QWidget
{
    Q_OBJECT
public:
    GridScene(QWidget* parent = 0);
    ~GridScene();
private:
    void paintEvent(QPaintEvent *);
    std::vector<std::vector<GridCell>> grid;
    static const int WIDTH = 50;
    static const int HEIGHT = 50;
    static const int CELL_SIZE = 10;
    static const int PADDING = 2;
};

#endif // GRIDSCENE_H
