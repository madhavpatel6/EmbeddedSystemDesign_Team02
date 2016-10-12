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
#include "gridcell.h"
#include "sensorclass.h"
#include "roverclass.h"
#include "gridhelper.h"

class GridScene : public QWidget
{
    Q_OBJECT
public:
    static std::vector<std::vector<GridCell>> grid;
    typedef enum { MIDDLESENSOR, RIGHTSENSOR, LEFTSENSOR, RIGHTSIDESENSOR, LEFTSIDESENSOR } SensorLocation;
    typedef enum { FIRSTCLICK, SECONDCLICK, ROTATE } MouseStateType;

    GridScene(QWidget* parent = 0);
    void reset();
    ~GridScene();
    void updateSensorReading();
    QPointF rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
//    static
    static bool checkBounds(double x1, double y1) {
        return y1 < HEIGHT && x1 < WIDTH && y1 >= 0 && x1 >= 0;
    }
    static const int WIDTH = 160;
    static const int HEIGHT = 160;
    static const int CELL_SIZE = 5;
    static const int PADDING = 2;
    bool showObjects;
signals:
    void updateCursorPosition(int x, int y);
    void updateRoverPosition(float x, float y, float newRectAngle);
private:
    void addRayTrace(SensorClass* impl);
    void addLine(double x1, double y1, double x2, double y2);
    SensorDataType getSensorData();
    MouseStateType mouseState;
    RoverClass* rover;
    SensorClass *middleFrontSensor;
    SensorClass *rightFrontSensor;
    SensorClass *leftFrontSensor;
    SensorClass *rightSideSensor;
    SensorClass *leftSideSensor;
    void initializeGrid();
    void paintEvent(QPaintEvent *);
    QVector<QPolygonF> rects;
    QPolygonF newRect;
    QPolygonF newRotatedRect;
    float newRectAngle;
    QPointF newRectCenter;
    QVector<QLineF> lines;
};

#endif // GRIDSCENE_H
