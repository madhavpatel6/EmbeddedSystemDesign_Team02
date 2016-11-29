#ifndef SENSORCLASS_H
#define SENSORCLASS_H

#include <QPainter>
#include <QPoint>
#include <QVector>
#include <QRect>
#include <QDebug>
#include "gridhelper.h"
class SensorClass
{
public:
    typedef struct ErrorType {
        float random;
        float bias;
    } ErrorType;
    typedef enum { MIDDLESENSOR, RIGHTSENSOR, LEFTSENSOR, RIGHTSIDESENSOR, LEFTSIDESENSOR, FARLEFTSENSOR, FARRIGHTSENSOR} SensorLocation;
    typedef enum { IRSENSOR, ULTRASONICSENSOR } SensorType;
    SensorClass(SensorType _sensorType, SensorLocation location, int _minimumDistanceCM, int _maximumDistanceCM, int _coneAngle, int _sensorOrientationOffset, QPointF roverLocation, int roverOrientation, int _cell_pixel_size);
    void updatePosition(QPointF roverLocation, int roverOrientation);
    float computeTotalError() {
        error.random = 0.5 - (float)rand()/(float)(RAND_MAX/1.0);
//        switch(type) {
//        case MIDDLESENSOR: {
//            qDebug() << "Middle Sensor Error: " << error.random + error.bias;
//            break;
//        }
//        case RIGHTSENSOR: {
//            qDebug() << "Right Sensor Error: " << error.random + error.bias;
//            break;
//        }
//        case LEFTSENSOR: {
//            qDebug() << "Left Sensor Error: " << error.random + error.bias;
//            break;
//        }
//        default:
//            break;
//        }
        return error.random + error.bias;
    }

    void draw(QPainter* painter);
    SensorData_t getParamAndDistance(QVector<QPolygonF> objs);
    float readDistance(QVector<QPolygonF> objs);
    QPointF getSensorLocation();
    int getSensorOrientation();
    QPointF getMaximumSensorLocation();
    bool simulateWithError;
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
    float sensorError;
    int cell_pixel_size;
    int maximumDistanceCM;
    int minimumDistanceCM;
    int coneAngle;
    int sensorOrientation;
    int sensorOrientationOffset;
    ErrorType error;
    SensorType sensorType;
};

#endif // SENSORCLASS_H
