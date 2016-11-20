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
    void resetGrid();
    void resetRoverPosition();
    ~GridScene();
    void updateSensorReading();
    QPointF rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
    void toggleError();
    QVector<QPolygonF> objects;
    QVector<int> keys;
    bool showObjects;
    bool recordKeys;
public slots:
    void handleUpdate(int row, QVector<char> vec) {
//        qDebug() << "Row " << row << " value " << vec;
        for(int i = 0; i < vec.size(); i++) {
            grid[row][i] = vec[i];
        }
        this->update();
    }

    void updateRoverPosition(float x, float y, float orientation) {
        rover->realLocationInformation.center.setX(x);
        rover->realLocationInformation.center.setY(y);
        rover->realLocationInformation.orientation = orientation;
        rover->estimatedLocationInformation = rover->realLocationInformation;
        middleFrontSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
        leftFrontSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
        rightFrontSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
        rightSideSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
        leftSideSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
        this->update();
    }
signals:
    void updateCursorPosition(int x, int y);
private:
    void addRayTrace(SensorClass* impl);
    void addLine(double x1, double y1, double x2, double y2);
    SensorDataContainerType getSensorData();
    MouseStateType mouseState;
    Grid::GridType grid;
    RoverClass* rover;
    SensorClass *middleFrontSensor;
    SensorClass *rightFrontSensor;
    SensorClass *leftFrontSensor;
    SensorClass *rightSideSensor;
    SensorClass *leftSideSensor;
    SensorClass *farLeftSensor;
    SensorClass *farRightSensor;
    void paintEvent(QPaintEvent *);
    QPolygonF newRect;
    QPolygonF newRotatedRect;
    float newRectAngle;
    QPointF newRectCenter;
    QVector<QLineF> lines;
};

#endif // GRIDSCENE_H
