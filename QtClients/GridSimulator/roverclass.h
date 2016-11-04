#ifndef ROVERCLASS_H
#define ROVERCLASS_H
#include <QPoint>
#include <QPainter>
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
    float computeMovementError() {
        movementError.random = 0.05/2 - (float)rand()/(float)(RAND_MAX/(0.1));
        return movementError.random + movementError.bias;
    }

    float computeRotationError() {
        rotationError.random =  0.0005 - (float)rand()/(float)(RAND_MAX/(0.001));
        return rotationError.random + rotationError.bias;
    }

    typedef struct ErrorType {
        float random;
        float bias;
    } ErrorType;
    void resetPosition() {
        estimatedLocationInformation = RoverLocation(QPoint(0,0),0);
        realLocationInformation = RoverLocation(QPoint(0,0),0);
    }

    void draw(QPainter* painter, int CELL_SIZE);
    void moveRoverUp(float distanceCM);
    void moveRoverBack(float distanceCM);
    void turnRoverRight(float angleDegrees);
    void turnRoverLeft(float angleDegrees);
    bool simulateWithError;
    RoverLocation getEstimatedLocationInformation() {
        return estimatedLocationInformation;
    }
    RoverLocation getRealLocationInformation() {
        return realLocationInformation;
    }
    RoverLocation estimatedLocationInformation;
    RoverLocation realLocationInformation;
private:
    ErrorType movementError;
    ErrorType rotationError;

};

#endif // ROVERCLASS_H
