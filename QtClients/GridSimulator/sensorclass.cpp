#include "sensorclass.h"
#include <cmath>
#include <QDebug>
#define M_PI 3.14159265358979323846
SensorClass::SensorClass(SensorType _sensorType, SensorLocation location, int _minimumDistanceCM, int _maximumDistanceCM, int _coneAngle, QPointF roverLocation, int roverOrientation, int _cell_pixel_size)
{
    cell_pixel_size = _cell_pixel_size;
    type = location;
    maximumDistanceCM = _maximumDistanceCM;
    coneAngle = _coneAngle;
    minimumDistanceCM = _minimumDistanceCM;
    updatePosition(roverLocation, roverOrientation);
    sensorType = _sensorType;
}

void SensorClass::updatePosition(QPointF roverLocation, int roverOrientation) {
    sensorOrientation = roverOrientation;
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

    }
    sensorMaximumLocation = QPointF(sensorLocation.x() + maximumDistanceCM*cos(sensorOrientation*M_PI/180.0),
                                   sensorLocation.y() + maximumDistanceCM*sin(sensorOrientation*M_PI/180.0));
}

void SensorClass::draw(QPainter* painter) {
    painter->drawEllipse(sensorPixelLocation, 3, 3);
    painter->save();
    painter->setPen(QPen(QBrush(Qt::black),3));
    for(int i = 0; i < temp.size(); i++) {
        painter->drawPoint(temp[i]);
    }
    painter->restore();
}

float SensorClass::readDistance(QVector<QRectF> objs) {
    switch(sensorType) {
        case IRSENSOR: {
            QPointF absoluteIntersectionPoint = findFirstIntersectionIR(objs);
            if(absoluteIntersectionPoint == sensorPixelLocation) {
                return -1;
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
                return -1;
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

QPointF SensorClass::findFirstIntersectionIR(QVector<QRectF> objs) {
    temp.clear();
    for(int i = 0; i < maximumDistanceCM*cell_pixel_size; i++) {
        QPointF checkPoint = QPointF ( i*cos(sensorOrientation*M_PI/180.0) + sensorPixelLocation.x(),
                                        i*1*sin(sensorOrientation*M_PI/180.0) + sensorPixelLocation.y());
        if(i > minimumDistanceCM * cell_pixel_size)
            temp.push_back(checkPoint);
        for(int j = 0; j < objs.size(); j++) {
            if(objs[j].contains(checkPoint)) {
                if(i < minimumDistanceCM * cell_pixel_size) {
                    return QPointF(-1,-1);
                }
                return checkPoint;
            }
        }
    }
    return sensorPixelLocation;
}


QPointF SensorClass::findFirstIntersectionUltrasonic(QVector<QRectF> objs) {
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
                if(objs[j].contains(coneCheckPoint)) {
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
