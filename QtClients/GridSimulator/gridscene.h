#ifndef GRIDSCENE_H
#define GRIDSCENE_H
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>
#include <QWidget>
#include "gridcell.h"
#include <vector>
#include <QLine>
#include <QRect>
#include <cmath>
#include "sensorclass.h"
typedef struct {
    QPoint center;
    int orientation;
} RoverLocationType;

class GridScene : public QWidget
{
    Q_OBJECT
public:
    typedef enum { MIDDLESENSOR, RIGHTSENSOR, LEFTSENSOR, RIGHTSIDESENSOR, LEFTSIDESENSOR } SensorLocation;
    typedef enum { FIRSTCORNER, SECONDCORNER, IDLE } MouseStateType;
    GridScene(QWidget* parent = 0);
    ~GridScene();
    void updateSensorReading(SensorLocation sensor, int distance);
    void addLine(float x1, float y1, float x2, float y2);
    QPoint rotatePoint(int originX, int originY, int pointX, int pointY, double rotationAngle);
    void raytrace(int x1, int y1, int const x2, int const y2);
    void raytrace2(double x0, double y0, double x1, double y1, bool maximum);
    void updateRoverLocation(QPoint center, int angle);
    void moveRoverUp(int distanceCM);
    void moveRoverBack(int distanceCM);
    void turnRoverRight(int angleDegrees);
    void turnRoverLeft(int angleDegrees);
    QPoint findFirstIntersection(SensorLocation sensor);
    void mouseMoveEvent(QMouseEvent *_event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
    static const int WIDTH = 100;
    static const int HEIGHT = 100;
    static const int CELL_SIZE = 5;
    static const int PADDING = 2;
    bool showObjects;
signals:
    void updateCursorPosition(int x, int y);

private:
    MouseStateType mouseState;
    RoverLocationType roverLocation;
    SensorClass *middleFrontSensor;
    SensorClass *rightFrontSensor;
    SensorClass *leftFrontSensor;

    void initializeGrid();
    void paintEvent(QPaintEvent *);
    std::vector<std::vector<GridCell>> grid;
    QVector<QRect> rects;
    QRect newRect;
    QVector<QLineF> lines;
};

#endif // GRIDSCENE_H
