#ifndef SENSORCLASS_H
#define SENSORCLASS_H

#include <QPainter>
#include <QPoint>
#include <QVector>
#include <QRect>
class SensorClass
{
public:
    typedef enum { MIDDLESENSOR, RIGHTSENSOR, LEFTSENSOR, RIGHTSIDESENSOR, LEFTSIDESENSOR } SensorLocation;
    typedef enum { IRSENSOR, ULTRASONICSENSOR } SensorType;
    SensorClass(SensorType _sensorType, SensorLocation location, int _minimumDistanceCM, int _maximumDistanceCM, int _coneAngle, QPointF roverLocation, int roverOrientation, int _cell_pixel_size);
    void updatePosition(QPointF roverLocation, int roverOrientation);
    void draw(QPainter* painter);
    float readDistance(QVector<QRectF> objs);
    QPointF getSensorLocation();
    int getSensorOrientation();
    QPointF getMaximumSensorLocation();
private:
    float computeDistance(QPointF p1, QPointF p2);
    QPointF rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle);
    QPointF findFirstIntersectionIR(QVector<QRectF> objs);
    QPointF findFirstIntersectionUltrasonic(QVector<QRectF> objs);
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
