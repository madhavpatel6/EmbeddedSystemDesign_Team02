#include "roverclass.h"
#include <QPoint>
#include <QDebug>
#include <cmath>

#define M_PI 3.14159265358979323846

RoverClass::RoverClass()
{
    estimatedLocationInformation = RoverLocation(QPoint(0,0),0);
    realLocationInformation = RoverLocation(QPoint(0,0),0);
    movementError.bias = 0.5 - (float)rand()/(float)(RAND_MAX/1);
    rotationError.bias = 0.25 - (float)rand()/(float)(RAND_MAX/.5);
    qDebug() << "Rover movement bias error: " << movementError.bias;
    qDebug() << "Rover rotation bias error: " << rotationError.bias;
    simulateWithError = false;
}

void RoverClass::moveRoverUp(float distanceCM) {
    float cosVal = cos(estimatedLocationInformation.orientation*M_PI/180.0);
    float sinVal = sin(estimatedLocationInformation.orientation*M_PI/180.0);
    QPointF newPosition = QPointF(estimatedLocationInformation.center.x() + cosVal * distanceCM,
                                estimatedLocationInformation.center.y() + sinVal * distanceCM);
    estimatedLocationInformation = RoverLocation(newPosition, estimatedLocationInformation.orientation);
    if(simulateWithError) {
        distanceCM += computeMovementError();
    }
    cosVal = cos(realLocationInformation.orientation*M_PI/180.0);
    sinVal = sin(realLocationInformation.orientation*M_PI/180.0);
    newPosition = QPointF(realLocationInformation.center.x() + cosVal * distanceCM,
                                realLocationInformation.center.y() + sinVal * distanceCM);
    realLocationInformation = RoverLocation(newPosition, realLocationInformation.orientation);
}

void RoverClass::moveRoverBack(float distanceCM) {
    float cosVal = cos(estimatedLocationInformation.orientation*M_PI/180.0);
    float sinVal = sin(estimatedLocationInformation.orientation*M_PI/180.0);
    QPointF newPosition = QPointF(estimatedLocationInformation.center.x() - cosVal * distanceCM,
                                estimatedLocationInformation.center.y() - sinVal * distanceCM);
    estimatedLocationInformation = RoverLocation(newPosition, estimatedLocationInformation.orientation);
    if(simulateWithError) {
        distanceCM += computeMovementError();
    }
    cosVal = cos(realLocationInformation.orientation*M_PI/180.0);
    sinVal = sin(realLocationInformation.orientation*M_PI/180.0);
    newPosition = QPointF(realLocationInformation.center.x() - cosVal * distanceCM,
                                realLocationInformation.center.y() - sinVal * distanceCM);
    realLocationInformation = RoverLocation(newPosition, realLocationInformation.orientation);
}

void RoverClass::turnRoverRight(float angleDegrees) {
    estimatedLocationInformation = RoverLocation(estimatedLocationInformation.center, estimatedLocationInformation.orientation - angleDegrees);
    if(simulateWithError) {
        angleDegrees += computeRotationError();
    }
    realLocationInformation = RoverLocation(realLocationInformation.center, realLocationInformation.orientation - angleDegrees);
}

void RoverClass::turnRoverLeft(float angleDegrees) {
    estimatedLocationInformation = RoverLocation(estimatedLocationInformation.center, estimatedLocationInformation.orientation + angleDegrees);
    if(simulateWithError) {
        angleDegrees += computeRotationError();
    }
    realLocationInformation = RoverLocation(realLocationInformation.center, realLocationInformation.orientation + angleDegrees);
}

void RoverClass::draw(QPainter *painter, int CELL_SIZE) {
    painter->save();
    // xc and yc are the center of the widget's rect.
    qreal xc = getEstimatedLocationInformation().center.x() * CELL_SIZE;
    qreal yc = getEstimatedLocationInformation().center.y() * CELL_SIZE;

    painter->setBrush(Qt::white);
    QPen pen;
    pen.setWidth(2);
    pen.setBrush(Qt::blue);
    painter->setPen(pen);

    // translates the coordinate system by xc and yc
    painter->translate(xc, yc);

    // then rotate the coordinate system by the number of degrees
    painter->rotate(getEstimatedLocationInformation().orientation);

    // we need to move the rectangle that we draw by rx and ry so it's in the center.

    painter->drawRect(QRect(-6*CELL_SIZE/2, -6*CELL_SIZE/2, 6*CELL_SIZE, 6*CELL_SIZE));
    painter->setBrush(Qt::black);


    painter->restore();
    painter->save();
    // xc and yc are the center of the widget's rect.
    xc = getRealLocationInformation().center.x() * CELL_SIZE;
    yc = getRealLocationInformation().center.y() * CELL_SIZE;

    painter->setBrush(Qt::white);
    pen.setBrush(Qt::red);
    painter->setPen(pen);

    // translates the coordinate system by xc and yc
    painter->translate(xc, yc);

    // then rotate the coordinate system by the number of degrees
    painter->rotate(getRealLocationInformation().orientation);

    // we need to move the rectangle that we draw by rx and ry so it's in the center.

    painter->drawRect(QRect(-6*CELL_SIZE/2, -6*CELL_SIZE/2, 6*CELL_SIZE, 6*CELL_SIZE));
    painter->setBrush(Qt::black);


    painter->restore();
}
