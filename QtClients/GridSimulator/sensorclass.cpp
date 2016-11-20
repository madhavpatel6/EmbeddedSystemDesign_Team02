#include "sensorclass.h"
#include <cmath>
#include <QDebug>
#include "gridhelper.h"
#include <time.h>
#include <random>
#define M_PI 3.14159265358979323846

SensorClass::SensorClass(SensorType _sensorType, SensorLocation location, int _minimumDistanceCM, int _maximumDistanceCM, int _coneAngle, int _sensorOrientationOffset, QPointF roverLocation, int roverOrientation, int _cell_pixel_size)
{
    cell_pixel_size = _cell_pixel_size;
    type = location;
    maximumDistanceCM = _maximumDistanceCM;
    coneAngle = _coneAngle;
    minimumDistanceCM = _minimumDistanceCM;
    sensorType = _sensorType;
    sensorOrientationOffset = _sensorOrientationOffset;
    error.bias = 2.0 - (float)rand()/(float)(RAND_MAX/4.0);
    switch(type) {
    case MIDDLESENSOR: {
        qDebug() << "Middle Sensor bias error " << error.bias;
        break;
    }
    case RIGHTSENSOR: {
        qDebug() << "Right Sensor bias error " << error.bias;
        break;
    }
    case LEFTSENSOR: {
        qDebug() << "Left Sensor bias error " << error.bias;
        break;
    }
    }
    simulateWithError = false;
    updatePosition(roverLocation, roverOrientation);
}

void SensorClass::updatePosition(QPointF roverLocation, int roverOrientation) {
    sensorOrientation = roverOrientation + sensorOrientationOffset;
    switch(type) {
        case MIDDLESENSOR: {
            sensorLocation = rotatePoint(roverLocation.x(), roverLocation.y(),
                    roverLocation.x() + 3, roverLocation.y(),
                        roverOrientation);
            sensorPixelLocation = rotatePoint(roverLocation.x()*cell_pixel_size, roverLocation.y()*cell_pixel_size,
                                              roverLocation.x()*cell_pixel_size + 3*cell_pixel_size, roverLocation.y()*cell_pixel_size,
                                                  roverOrientation);

            break;
        }
        case LEFTSENSOR: {
            sensorLocation = rotatePoint(roverLocation.x(), roverLocation.y(),
                    roverLocation.x() + 3, roverLocation.y() + 2,
                        roverOrientation);
            sensorPixelLocation = rotatePoint(roverLocation.x()*cell_pixel_size, roverLocation.y()*cell_pixel_size,
                                              roverLocation.x()*cell_pixel_size + 3*cell_pixel_size, roverLocation.y()*cell_pixel_size + 2*cell_pixel_size,
                                                  roverOrientation);
            break;
        }
        case RIGHTSENSOR: {
            sensorLocation = rotatePoint(roverLocation.x(), roverLocation.y(),
                    roverLocation.x() + 3, roverLocation.y() - 2,
                        roverOrientation);
            sensorPixelLocation = rotatePoint(roverLocation.x()*cell_pixel_size, roverLocation.y()*cell_pixel_size,
                                              roverLocation.x()*cell_pixel_size + 3*cell_pixel_size, roverLocation.y()*cell_pixel_size - 2*cell_pixel_size,
                                                  roverOrientation);
            break;
        }
        case RIGHTSIDESENSOR: {
            sensorOrientation = roverOrientation - 90;
            sensorLocation = rotatePoint(roverLocation.x(), roverLocation.y(),
                    roverLocation.x(), roverLocation.y() - 3,
                        roverOrientation);
            sensorPixelLocation = rotatePoint(roverLocation.x()*cell_pixel_size, roverLocation.y()*cell_pixel_size,
                                              roverLocation.x()*cell_pixel_size, roverLocation.y()*cell_pixel_size - 3*cell_pixel_size,
                                                  roverOrientation);
            break;
        }
        case LEFTSIDESENSOR: {
            sensorOrientation = roverOrientation + 90;
            sensorLocation = rotatePoint(roverLocation.x(), roverLocation.y(),
                    roverLocation.x(), roverLocation.y() + 3,
                        roverOrientation);
            sensorPixelLocation = rotatePoint(roverLocation.x()*cell_pixel_size, roverLocation.y()*cell_pixel_size,
                                              roverLocation.x()*cell_pixel_size, roverLocation.y()*cell_pixel_size + 3*cell_pixel_size,
                                                  roverOrientation);
            break;
        }
        case FARLEFTSENSOR: {
            sensorLocation = rotatePoint(roverLocation.x(), roverLocation.y(),
                    roverLocation.x() + 3, roverLocation.y() + 4,
                        roverOrientation);
            sensorPixelLocation = rotatePoint(roverLocation.x()*cell_pixel_size, roverLocation.y()*cell_pixel_size,
                                              roverLocation.x()*cell_pixel_size + 3*cell_pixel_size, roverLocation.y()*cell_pixel_size + 4*cell_pixel_size,
                                                  roverOrientation);
            break;
        }
        case FARRIGHTSENSOR: {
            sensorLocation = rotatePoint(roverLocation.x(), roverLocation.y(),
                    roverLocation.x() + 3, roverLocation.y() - 4,
                        roverOrientation);
            sensorPixelLocation = rotatePoint(roverLocation.x()*cell_pixel_size, roverLocation.y()*cell_pixel_size,
                                              roverLocation.x()*cell_pixel_size + 3*cell_pixel_size, roverLocation.y()*cell_pixel_size - 4*cell_pixel_size,
                                                  roverOrientation);
            break;
        }

    }
    sensorMaximumLocation = QPointF(sensorLocation.x() + maximumDistanceCM*cos(sensorOrientation*M_PI/180.0),
                                   sensorLocation.y() + maximumDistanceCM*sin(sensorOrientation*M_PI/180.0));
}

void SensorClass::draw(QPainter* painter) {
    painter->drawEllipse(sensorPixelLocation, 3, 3);
    painter->save();
    painter->setPen(QPen(QBrush(Qt::red),3));
    for(int i = 0; i < temp.size(); i++) {
        painter->drawPoint(temp[i]);
    }
    painter->restore();
}

SensorData_t SensorClass::getParamAndDistance(QVector<QPolygonF> objs) {
    SensorData_t ret;
    ret.coneAngle = this->coneAngle;
    ret.distance = readDistance(objs);
    if(simulateWithError && ret.distance != -1 && ret.distance != -2) {
        ret.distance = ret.distance + computeTotalError();
    }
    ret.orientation = this->sensorOrientation;
    ret.sensorLocation = this->sensorLocation;
    ret.maxSensorLocation = this->getMaximumSensorLocation();
    ret.minimumMeasuringDistance = this->minimumDistanceCM;
    ret.maximumMeasuringDistance = this->maximumDistanceCM;
    return ret;
}

float SensorClass::readDistance(QVector<QPolygonF> objs) {
    switch(sensorType) {
        case IRSENSOR: {
            QPointF absoluteIntersectionPoint = findFirstIntersectionIR(objs);
            if(absoluteIntersectionPoint == sensorPixelLocation) {
                return maximumDistanceCM;
            }
            else if (absoluteIntersectionPoint == QPointF(-1,-1)) {
                return -2;
            }
            else {
                float absoluteDistance = computeDistance(absoluteIntersectionPoint, sensorPixelLocation);
                return absoluteDistance/cell_pixel_size;
            }
            break;
        }
        case ULTRASONICSENSOR: {
            QPointF absoluteIntersectionPoint = findFirstIntersectionUltrasonic(objs);
            if(absoluteIntersectionPoint == sensorPixelLocation) {
                return maximumDistanceCM;
            }
            else if (absoluteIntersectionPoint == QPointF(-1,-1)) {
                return -2;
            }
            else {
                float absoluteDistance = computeDistance(absoluteIntersectionPoint, sensorPixelLocation);
                return absoluteDistance/cell_pixel_size;
            }
            break;
        }
    }
    return -2;
}


int SensorClass::getSensorOrientation() {
    return sensorOrientation;
}

QPointF SensorClass::getSensorLocation() {
    return sensorLocation;
}

QPointF SensorClass::getMaximumSensorLocation() {
    return sensorMaximumLocation;
}

float SensorClass::computeDistance(QPointF p1, QPointF p2) {
    return floor(sqrt( pow(p1.x() - p2.x(),2) + pow(p1.y() - p2.y(),2) ));
}

QPointF SensorClass::findFirstIntersectionIR(QVector<QPolygonF> objs) {
    temp.clear();
    int alternate = 0;
    for(int i = 0; i <= maximumDistanceCM*cell_pixel_size; i++) {
        QPointF checkPoint = QPointF ( i*cos(sensorOrientation*M_PI/180.0) + sensorPixelLocation.x(),
                                        i*1*sin(sensorOrientation*M_PI/180.0) + sensorPixelLocation.y());
        if(i > minimumDistanceCM * cell_pixel_size && alternate == 10 || i == maximumDistanceCM*cell_pixel_size)
            temp.push_back(checkPoint);
        for(int j = 0; j < objs.size(); j++) {
            if(objs[j].containsPoint(checkPoint, Qt::WindingFill)) {
                if(i < minimumDistanceCM * cell_pixel_size) {
                    return QPointF(-1,-1);
                }
                return checkPoint;
            }
        }
        if(alternate == 10) {
            alternate = 0;
        }
        else {
            alternate++;
        }
    }
    return sensorPixelLocation;
}


QPointF SensorClass::findFirstIntersectionUltrasonic(QVector<QPolygonF> objs) {
    temp.clear();
    int alternate = 0;
    for(int i = 0; i <= maximumDistanceCM*cell_pixel_size; i++) {
        QPointF centerCheckPoint = QPointF ( i*cos(sensorOrientation*M_PI/180.0) + sensorPixelLocation.x(),
                                        i*1*sin(sensorOrientation*M_PI/180.0) + sensorPixelLocation.y());
        for(double l = -coneAngle/2.0; l <= coneAngle / 2.0; l = l + .5) {
            QPointF coneCheckPoint = rotatePoint(sensorPixelLocation.x(), sensorPixelLocation.y(), centerCheckPoint.x(), centerCheckPoint.y(), l);
            if(((l == 0 || l == -coneAngle/2.0 || l == coneAngle/2.0 || centerCheckPoint == coneCheckPoint) && alternate == 10 || i == maximumDistanceCM*cell_pixel_size || i == minimumDistanceCM*cell_pixel_size) && i >= minimumDistanceCM * cell_pixel_size)
                temp.push_back(coneCheckPoint);
            for(int j = 0; j < objs.size(); j++) {
                if(objs[j].containsPoint(coneCheckPoint, Qt::WindingFill)) {
                    if(i < minimumDistanceCM * cell_pixel_size) {
                        return QPointF(-1,-1);
                    }
                    return coneCheckPoint;
                }
            }
        }
        if(alternate == 10) {
            alternate = 0;
        }
        else {
            alternate++;
        }
    }
    return sensorPixelLocation;
}

QPointF SensorClass::rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle) {
    float translatedX = pointX - originX;
    float translatedY = pointY - originY;
    rotationAngle = rotationAngle*M_PI/180.0;
    QPointF point = QPointF(translatedX * cos(rotationAngle) - translatedY * sin(rotationAngle) + originX,
                  translatedX * sin(rotationAngle) + translatedY * cos(rotationAngle) + originY);
//    qDebug() << "new Point " << point;
    return point;
}
