#ifndef GRIDSCENE_H
#define GRIDSCENE_H
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>
#include <QWidget>
#include <vector>
#include <QLine>
#include <QRect>
#include <QPolygonF>
#include <cmath>
#include "grid.h"
#include "sensorclass.h"
#include "roverclass.h"
#include "gridhelper.h"

class GridScene : public QWidget
{
    Q_OBJECT
public:
    typedef enum { MIDDLESENSOR, RIGHTSENSOR, LEFTSENSOR, RIGHTSIDESENSOR, LEFTSIDESENSOR } SensorLocation;
    typedef enum { FIRSTCLICK, SECONDCLICK, ROTATE } MouseStateType;
    static const int CELL_SIZE = 5;
    static const int PADDING = 2;
    GridScene(QWidget* parent = 0);
    void reset();
    ~GridScene();
    void updateSensorReading();
    QPointF rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
    bool showObjects;
signals:
    void updateCursorPosition(int x, int y);
    void updateRoverPosition(float x, float y, float newRectAngle);
private:
    void addRayTrace(SensorClass* impl);
    void addLine(double x1, double y1, double x2, double y2);
    SensorDataType getSensorData();
    MouseStateType mouseState;
    Grid::GridType grid;
    RoverClass* rover;
    SensorClass *middleFrontSensor;
    SensorClass *rightFrontSensor;
    SensorClass *leftFrontSensor;
    SensorClass *rightSideSensor;
    SensorClass *leftSideSensor;
    void paintEvent(QPaintEvent *);
    QVector<QPolygonF> rects;
    QPolygonF newRect;
    QPolygonF newRotatedRect;
    float newRectAngle;
    QPointF newRectCenter;
    QVector<QLineF> lines;
};

#endif // GRIDSCENE_H
