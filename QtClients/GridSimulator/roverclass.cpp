#include "roverclass.h"
#include <QPoint>
#include <QDebug>
RoverClass::RoverClass()
{
    locationInformation = RoverLocation(QPoint(50,50),0);
}

void RoverClass::moveRoverUp(float distanceCM) {
    float cosVal = cos(locationInformation.orientation*M_PI/180.0);
    float sinVal = sin(locationInformation.orientation*M_PI/180.0);
    QPointF newPosition = QPointF(locationInformation.center.x() + cosVal * distanceCM,
                                locationInformation.center.y() + sinVal * distanceCM);
    locationInformation = RoverLocation(newPosition, locationInformation.orientation);
}

void RoverClass::moveRoverBack(float distanceCM) {
    float cosVal = cos(locationInformation.orientation*M_PI/180.0);
    float sinVal = sin(locationInformation.orientation*M_PI/180.0);
    cosVal = cosVal < 0.00001 ? 0 : cosVal;
    sinVal = sinVal < 0.00001 ? 0 : sinVal;
    QPointF newPosition = QPointF(locationInformation.center.x() - cosVal * distanceCM,
                                locationInformation.center.y() - sinVal * distanceCM);
    locationInformation = RoverLocation(newPosition, locationInformation.orientation);
}

void RoverClass::turnRoverRight(float angleDegrees) {
    locationInformation = RoverLocation(locationInformation.center, locationInformation.orientation - angleDegrees);
}

void RoverClass::turnRoverLeft(float angleDegrees) {
    locationInformation = RoverLocation(locationInformation.center, locationInformation.orientation + angleDegrees);
}
