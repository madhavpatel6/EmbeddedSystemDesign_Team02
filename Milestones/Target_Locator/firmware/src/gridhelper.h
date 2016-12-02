#ifndef GRIDHELPER_H
#define GRIDHELPER_H

#include "grid.h"

typedef struct {
//    float minimumMeasuringDistance;
//    float maximumMeasuringDistance;
    float distance;
    float orientation;
    point_t sensorLocation;
} SensorData_t;

typedef struct {
    bool LongRangeIsValid;
    bool MidRangeIsValid;
    SensorData_t middleFrontSensor;
    SensorData_t rightFrontSensor;
    SensorData_t leftFrontSensor;
    SensorData_t farRightSensor;
    SensorData_t farLeftSensor;
    point_t roverLocation;
    float orientation;
} SensorDataContainerType;

void rotatePoint(point_t* point, float originX, float originY, float pointX, float pointY, double rotationAngle);

void raytrace3(int x1, int y1, int x2, int y2, bool maximum, GridType grid, GridUpdatedType updated);

void updateOccupanyGrid3(SensorDataContainerType sensorData, GridType grid, GridUpdatedType updated);

#endif // GRIDHELPER_H
