#ifndef SENSORCLASS_H
#define SENSORCLASS_H

#include <QPainter>
#include <QPoint>
#include <QVector>
#include <QRect>
#include "gridhelper.h"
class SensorClass
{
public:
    typedef enum { MIDDLESENSOR, RIGHTSENSOR, LEFTSENSOR, RIGHTSIDESENSOR, LEFTSIDESENSOR } SensorLocation;
    typedef enum { IRSENSOR, ULTRASONICSENSOR } SensorType;
    SensorClass(SensorType _sensorType, SensorLocation location, int _minimumDistanceCM, int _maximumDistanceCM, int _coneAngle, QPointF roverLocation, int roverOrientation, int _cell_pixel_size);
    void updatePosition(QPointF roverLocation, int roverOrientation);
    void draw(QPainter* painter);
    SensorData_t getParamAndDistance(QVector<QPolygonF> objs);
    float readDistance(QVector<QPolygonF> objs);
    QPointF getSensorLocation();
    int getSensorOrientation();
    QPointF getMaximumSensorLocation();
private:
    float computeDistance(QPointF p1, QPointF p2);
    QPointF rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle);
    QPointF findFirstIntersectionIR(QVector<QPolygonF> objs);
    QPointF findFirstIntersectionUltrasonic(QVector<QPolygonF> objs);
    SensorLocation type;
    QPointF sensorLocation;
    QPointF sensorPixelLocation;
    QPointF sensorMaximumLocation;
    QVector<QPointF> temp;
    int sensorOrientation;
    int cell_pixel_size;
    int maximumDistanceCM;
    int minimumDistanceCM;
    int coneAngle;
    SensorType sensorType;
};

#endif // SENSORCLASS_H
