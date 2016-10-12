#ifndef GRIDHELPER_H
#define GRIDHELPER_H

#include <QPointF>

typedef struct {
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
} SensorDataType;

namespace GridHelper {

void raytrace(double x1, double y1, double x2, double y2);

void raytrace2(double x0, double y0, double x1, double y1, bool maximum);

void raytrace3(int x1, int y1, int x2, int y2);

void updateOccupanyGrid(SensorDataType sensorData);

}
#endif // GRIDHELPER_H
