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
    SensorClass(QPoint roverLocation, int roverOrientation, int _maximumDistanceCM, int _cell_pixel_size, SensorLocation location);
    void updatePosition(QPoint roverLocation, int roverOrientation);
    void draw(QPainter* painter);
    int readDistance(QVector<QRect> objs);
    QPoint getSensorLocation();
    int getSensorOrientation();
    QPoint getMaximumSensorLocation();
private:
    int computeDistance(QPoint p1, QPoint p2);
    QPoint rotatePoint(int originX, int originY, int pointX, int pointY, double rotationAngle);
    QPoint findFirstIntersection(QVector<QRect> objs);
    SensorLocation type;
    QPoint sensorLocation;
    QPoint sensorPixelLocation;
    QPoint sensorMaximumLocation;
    QVector<QPoint> temp;
    int sensorOrientation;
    int cell_pixel_size;
    int maximumDistanceCM;
};

#endif // SENSORCLASS_H
