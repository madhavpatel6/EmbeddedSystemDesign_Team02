#ifndef GRIDHELPER_H
#define GRIDHELPER_H

#include <QPointF>
#include <QPoint>
#include "grid.h"

typedef struct {
    float minimumMeasuringDistance;
    float maximumMeasuringDistance;
    float distance;
    float orientation;
    float coneAngle;
    QPointF sensorLocation;
    QPointF maxSensorLocation;
} SensorData_t;

typedef struct {
    SensorData_t middleFrontSensor;
    SensorData_t rightFrontSensor;
    SensorData_t leftFrontSensor;
    SensorData_t rightSideSensor;
    SensorData_t leftSideSensor;
} SensorDataContainerType;

typedef struct {
    int number;
    QVector<QPoint> decrementPoints;
    QVector<QPoint> incrementPoints;
} RayTraceReturnType;

namespace GridHelper {

RayTraceReturnType raytrace(double x1, double y1, double x2, double y2,bool maximum, Grid::GridType grid);

RayTraceReturnType raytrace2(double x0, double y0, double x1, double y1, bool maximum, Grid::GridType grid);

RayTraceReturnType raytrace3(int x1, int y1, int x2, int y2, bool maximum, Grid::GridType grid);

void updateOccupanyGrid(SensorDataContainerType sensorData, Grid::GridType grid);

}
#endif // GRIDHELPER_H
