#include "sensorclass.h"
#include <cmath>
#include <QDebug>
#define M_PI 3.14159265358979323846
SensorClass::SensorClass(QPointF roverLocation, int roverOrientation, int _maximumDistanceCM, int _cell_pixel_size, SensorLocation location)
{
    cell_pixel_size = _cell_pixel_size;
    type = location;
    maximumDistanceCM = _maximumDistanceCM;
    updatePosition(roverLocation, roverOrientation);
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
    QPointF absoluteIntersectionPoint = findFirstIntersection(objs);
//    qDebug() << absoluteIntersectionPoint;
    if(absoluteIntersectionPoint != sensorPixelLocation) {
        float absoluteDistance = computeDistance(absoluteIntersectionPoint, sensorPixelLocation);
//        qDebug() << "Distance found " << absoluteDistance/cell_pixel_size;
        return absoluteDistance/cell_pixel_size;
    }
    return -1;
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

QPointF SensorClass::findFirstIntersection(QVector<QRectF> objs) {
//    qDebug() << "Sensor location " << sensorLocation << " orientation " << sensorOrientation;
    temp.clear();
    for(int i = 0; i < maximumDistanceCM*cell_pixel_size; i++) {
        QPointF checkPoint = QPointF ( i*cos(sensorOrientation*M_PI/180.0) + sensorPixelLocation.x(),
                                        i*1*sin(sensorOrientation*M_PI/180.0) + sensorPixelLocation.y());
//        qDebug() << "check point = " << checkPoint;
        temp.push_back(checkPoint);
        for(int j = 0; j < objs.size(); j++) {
//            qDebug() << "checking object " << objs[j];
            if(objs[j].contains(checkPoint)) {
                return checkPoint;
            }
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
