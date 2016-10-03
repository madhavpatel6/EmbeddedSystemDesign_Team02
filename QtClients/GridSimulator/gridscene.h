#ifndef GRIDSCENE_H
#define GRIDSCENE_H
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>
#include <QWidget>
#include "gridcell.h"
#include <vector>
#include <QLine>
#include <cmath>

typedef struct {
    QPoint location;
    int orientation;
} RoverLocationType;

class GridScene : public QWidget
{
    Q_OBJECT
public:
    GridScene(QWidget* parent = 0);
    ~GridScene();
    void addLine(float x1, float y1, float x2, float y2);
    void raytrace(int x1, int y1, int const x2, int const y2);
    void raytrace2(double x0, double y0, double x1, double y1);
private:
    RoverLocationType roverLocation;
    void initializeGrid();
    void paintEvent(QPaintEvent *);
    std::vector<std::vector<GridCell>> grid;
    QVector<QLineF> lines;
    static const int WIDTH = 100;
    static const int HEIGHT = 100;
    static const int CELL_SIZE = 10;
    static const int PADDING = 2;
};

#endif // GRIDSCENE_H
