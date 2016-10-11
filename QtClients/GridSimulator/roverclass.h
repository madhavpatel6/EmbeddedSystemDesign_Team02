#ifndef ROVERCLASS_H
#define ROVERCLASS_H
#include <QPoint>

typedef struct RoverLocation {
    QPointF center;
    float orientation;
    RoverLocation() : center(QPointF(0,0)) , orientation(0) {}
    RoverLocation(QPointF newCenter, float newOrientation) : center(newCenter), orientation(newOrientation) { }
} RoverLocation;

class RoverClass
{
public:
    RoverClass();
    void moveRoverUp(float distanceCM);
    void moveRoverBack(float distanceCM);
    void turnRoverRight(float angleDegrees);
    void turnRoverLeft(float angleDegrees);
    void updateRoverPosition(QPointF location, float newOrientation);
    RoverLocation getLocationInformation() {
        return locationInformation;
    }
private:
    RoverLocation locationInformation;
};

#endif // ROVERCLASS_H
