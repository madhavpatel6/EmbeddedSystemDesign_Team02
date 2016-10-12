#ifndef GRIDHELPER_H
#define GRIDHELPER_H


typedef struct {
    float middleFrontSensorDistance;
    float rightFrontSensorDistance;
    float leftFrontSensorDistance;
    float rightSideSensorDistance;
    float leftSideSensorDistance;
} SensorDataType;

namespace GridHelper {

void raytrace(double x1, double y1, double x2, double y2);

void raytrace2(double x0, double y0, double x1, double y1, bool maximum);

void raytrace3(int x1, int y1, int x2, int y2);

void updateOccupanyGrid(SensorDataType sensorData);

}
#endif // GRIDHELPER_H
