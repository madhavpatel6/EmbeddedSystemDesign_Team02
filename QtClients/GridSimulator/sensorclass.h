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
    SensorClass(QPointF roverLocation, int roverOrientation, int _minimumDistanceCM, int _maximumDistanceCM, int _cell_pixel_size, SensorLocation location);
    void updatePosition(QPointF roverLocation, int roverOrientation);
    void draw(QPainter* painter);
    float readDistance(QVector<QRectF> objs);
    QPointF getSensorLocation();
    int getSensorOrientation();
    QPointF getMaximumSensorLocation();
private:
    float computeDistance(QPointF p1, QPointF p2);
    QPointF rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle);
    QPointF findFirstIntersection(QVector<QRectF> objs);
    SensorLocation type;
    QPointF sensorLocation;
    QPointF sensorPixelLocation;
    QPointF sensorMaximumLocation;
    QVector<QPointF> temp;
    int sensorOrientation;
    int cell_pixel_size;
    int maximumDistanceCM;
    int minimumDistanceCM;
};

#endif // SENSORCLASS_H
