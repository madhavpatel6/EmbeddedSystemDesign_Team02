#ifndef GRIDHELPER_H
#define GRIDHELPER_H

#include "grid.h"


typedef struct PointF {
    float x;
    float y;
} PointF;

typedef PointF point_t;

typedef struct {
    float minimumMeasuringDistance;
    float maximumMeasuringDistance;
    float distance;
    float orientation;
    point_t sensorLocation;
} SensorData_t;

typedef struct {
    SensorData_t middleFrontSensor;
    SensorData_t rightFrontSensor;
    SensorData_t leftFrontSensor;
    point_t roverLocation;
    float orientation;
} SensorDataContainerType;

void rotatePoint(point_t* point, float originX, float originY, float pointX, float pointY, double rotationAngle);

void raytrace3(int x1, int y1, int x2, int y2, bool maximum, GridType grid);

void updateOccupanyGrid2(SensorDataContainerType sensorData, GridType grid);

#endif // GRIDHELPER_H
