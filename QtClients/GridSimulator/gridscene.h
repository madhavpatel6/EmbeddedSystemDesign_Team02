#ifndef GRIDSCENE_H
#define GRIDSCENE_H
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>
#include <QWidget>
#include <vector>
#include <QLine>
#include <QRect>
#include <cmath>
#include "gridcell.h"
#include "sensorclass.h"
#include "roverclass.h"

class GridScene : public QWidget
{
    Q_OBJECT
public:
    typedef enum { MIDDLESENSOR, RIGHTSENSOR, LEFTSENSOR, RIGHTSIDESENSOR, LEFTSIDESENSOR } SensorLocation;
    typedef enum { FIRSTCORNER, SECONDCORNER, IDLE } MouseStateType;
    GridScene(QWidget* parent = 0);
    void reset();
    ~GridScene();
    void updateSensorReading();
    void addRayTrace(SensorClass* impl);
    void addLine(double x1, double y1, double x2, double y2);
    QPointF rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle);
    void raytrace(double x1, double y1, double x2, double y2);
    void raytrace2(double x0, double y0, double x1, double y1, bool maximum);
    void raytrace3(int x1, int y1, int x2, int y2);
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
    bool checkBounds(double x1, double y1) {
        return y1 < HEIGHT && x1 < WIDTH && y1 >= 0 && x1 >= 0;
    }

    MouseStateType mouseState;
    RoverClass* rover;
    SensorClass *middleFrontSensor;
    SensorClass *rightFrontSensor;
    SensorClass *leftFrontSensor;
    SensorClass *rightSideSensor;
    SensorClass *leftSideSensor;
    void initializeGrid();
    void paintEvent(QPaintEvent *);
    std::vector<std::vector<GridCell>> grid;
    QVector<QRectF> rects;
    QRectF newRect;
    QVector<QLineF> lines;
};

#endif // GRIDSCENE_H
