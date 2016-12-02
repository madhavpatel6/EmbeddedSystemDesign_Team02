#include <iostream>
#include "gridhelper.h"
#include "Grid.h"
#include <QDebug>
#define M_PI 3.14159265358979323846

namespace GridHelper {

/**
 * @brief raytrace
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param maximum
 * @param grid
 * @return
 */
RayTraceReturnType raytrace(double x1, double y1, double x2, double y2, bool maximum, Grid::GridType grid)
{
    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix(((delta_x > 0) - (delta_x < 0)));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy(((delta_y > 0) - (delta_y < 0)));
    delta_y = std::abs(delta_y) << 1;

    //plot(x1, y1);

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));

        while (floor(x1) != floor(x2))
        {
            if(Grid::checkBounds(x1, y1)) {
                qDebug() << "(x,y) = (" << x1 << ", " << y1 <<")";
                Grid::decrementIndex(x1, y1, grid);
            }
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing

            error += delta_y;
            x1 += ix;
            //plot(x1, y1);
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
        while (floor(y1) != floor(y2))
        {
            if(Grid::checkBounds(x1, y1)) {
                qDebug() << "(x,y) = (" << x1 << ", " << y1 <<")";
                Grid::decrementIndex(x1, y1, grid);
            }
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing

            error += delta_x;
            y1 += iy;
        }
    }
    RayTraceReturnType ret;
    return ret;
}

/**
 * @brief raytrace2
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param maximum
 * @param grid
 * @return
 */
RayTraceReturnType raytrace2(double x0, double y0, double x1, double y1, bool maximum, Grid::GridType grid)
{
    RayTraceReturnType ret;
    double dx = fabs(x1 - x0);
    double dy = fabs(y1 - y0);
    x0 = x0;
    y0 = y0;
    x1 = x1;
    y1 = y1;
    int x = int(floor(x0));
    int y = int(floor(y0));

    double dt_dx = 1.0 / dx;
    double dt_dy = 1.0 / dy;
    double t = 0;

    int n = 1;
    int x_inc, y_inc;
    double t_next_vertical, t_next_horizontal;

    if (dx == 0)
    {
        x_inc = 0;
        t_next_horizontal = dt_dx; // infinity
    }
    else if (x1 > x0)
    {
        x_inc = 1;
        n += int(floor(x1)) - x;
        t_next_horizontal = (floor(x0) + 1 - x0) * dt_dx;
    }
    else
    {
        x_inc = -1;
        n += x - int(floor(x1));
        t_next_horizontal = (x0 - floor(x0)) * dt_dx;
    }

    if (dy == 0)
    {
        y_inc = 0;
        t_next_vertical = dt_dy; // infinity
    }
    else if (y1 > y0)
    {
        y_inc = 1;
        n += int(floor(y1)) - y;
        t_next_vertical = (floor(y0) + 1 - y0) * dt_dy;
    }
    else
    {
        y_inc = -1;
        n += y - int(floor(y1));
        t_next_vertical = (y0 - floor(y0)) * dt_dy;
    }
    ret.number = n;
    qDebug() << "n = " << n;
    for (; n > 0; --n)
    {
        if(Grid::checkBounds(y, x)) {
            if(n == 1 && !maximum) {
                ret.incrementPoints.push_back(QPoint(x,y));
                qDebug() << "maximum = (x,y) = (" << x << ", " << y <<")";
                Grid::incrementIndex(x, y, grid);
            }
            else{
                ret.decrementPoints.push_back(QPoint(x,y));
                qDebug() << "(x,y) = (" << x << ", " << y <<")";
                Grid::decrementIndex(x, y, grid);
            }
        }
        if (t_next_vertical < t_next_horizontal)
        {
            y += y_inc;
            t = t_next_vertical;
            t_next_vertical += dt_dy;
        }
        else
        {
            x += x_inc;
            t = t_next_horizontal;
            t_next_horizontal += dt_dx;
        }
    }
    return ret;
}

/**
 * @brief raytrace3
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param maximum
 * @param grid
 * @return
 */
RayTraceReturnType raytrace3(int x1, int y1, int x2, int y2, bool maximum, Grid::GridType grid)
{
    RayTraceReturnType ret;
    int i;               // loop counter
    int ystep, xstep;    // the step on y and x axis
    int error;           // the error accumulated during the increment
    int errorprev;       // *vision the previous value of the error variable
    int y = y1, x = x1;  // the line points
    int ddy, ddx;        // compulsory variables: the double values of dy and dx
    int dx = x2 - x1;
    int dy = y2 - y1;
    if(Grid::checkBounds(x, y)) {
        ret.decrementPoints.push_back(QPoint(x,y));
        Grid::decrementIndex(x, y, grid);
    }
    // NB the last point can't be here, because of its previous point (which has to be verified)
    if (dy < 0){
        ystep = -1;
        dy = -dy;
    }
    else {
        ystep = 1;
    }
    if (dx < 0) {
        xstep = -1;
        dx = -dx;
    }
    else {
        xstep = 1;
    }
    ddy = 2 * dy;  // work with double values for full precision
    ddx = 2 * dx;

    if (ddx >= ddy) {  // first octant (0 <= slope <= 1)
    // compulsory initialization (even for errorprev, needed when dx==dy)
        errorprev = error = dx;  // start in the middle of the square
        for (i=0 ; i < dx ; i++) {  // do not use the first point (already done)
          x += xstep;
          error += ddy;
          if (error > ddx){  // increment y if AFTER the middle ( > )
            y += ystep;
            error -= ddx;
            // three cases (octant == right->right-top for directions below):
            if (error + errorprev < ddx && Grid::checkBounds(x, y)) {  // bottom square als
                ret.decrementPoints.push_back(QPoint(x,y));
                Grid::decrementIndex(x, y, grid);
            }
            else if (error + errorprev > ddx && Grid::checkBounds(x-xstep, y)) { // left square also
                ret.decrementPoints.push_back(QPoint(x-xstep,y));
                Grid::decrementIndex(x-xstep, y, grid);
            }
            else if(Grid::checkBounds(x, y-ystep) && Grid::checkBounds(x-xstep, y)){  // corner: bottom and left squares also
                ret.decrementPoints.push_back(QPoint(x-xstep,y));
                ret.decrementPoints.push_back(QPoint(x,y-ystep));
                Grid::decrementIndex(x, y-ystep, grid);
                Grid::decrementIndex(x-xstep, y, grid);
            }
          }
          if(Grid::checkBounds(x, y)) {
              if(!maximum && i+1 == dx) {
                    ret.incrementPoints.push_back(QPoint(x,y));
                    Grid::incrementIndex(x, y, grid);
              }
              else {
                  ret.decrementPoints.push_back(QPoint(x,y));
                    Grid::decrementIndex(x, y, grid);
              }
          }
          errorprev = error;
        }
    }
    else {  // the same as above
        errorprev = error = dy;
        for (i=0 ; i < dy ; i++) {
            y += ystep;
            error += ddx;
            if (error > ddy) {
                x += xstep;
                error -= ddy;
                if (error + errorprev < ddy && Grid::checkBounds(x-xstep, y)) {
                    ret.decrementPoints.push_back(QPoint(x-xstep,y));
                    Grid::decrementIndex(x-xstep,y,grid);
                }
                else if (error + errorprev > ddy && Grid::checkBounds(x, y-ystep)) {
                    ret.decrementPoints.push_back(QPoint(x,y-ystep));
                    Grid::decrementIndex(x, y-ystep, grid);
                }
                else if (Grid::checkBounds(x, y-ystep) && Grid::checkBounds(x-xstep, y)){
                    ret.decrementPoints.push_back(QPoint(x-xstep,y));
                    ret.decrementPoints.push_back(QPoint(x,y-ystep));
                    Grid::decrementIndex(x-xstep,y,grid);
                    Grid::decrementIndex(x, y-ystep, grid);
                }
            }
            if(Grid::checkBounds(x, y)) {
                if(!maximum && i+1 == dy) {
                    ret.incrementPoints.push_back(QPoint(x,y));
                    Grid::incrementIndex(x, y, grid);
                }
                else {
                    ret.decrementPoints.push_back(QPoint(x,y));
                    Grid::decrementIndex(x,y,grid);
                }
                errorprev = error;
            }
        }
    }
    return ret;
  // assert ((y == y2) && (x == x2));  // the last point (y2,x2) has to be the same with the last point of the algorithm
}

point_t rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle) {
    int translatedX = pointX - originX;
    int translatedY = pointY - originY;
    rotationAngle = rotationAngle*M_PI/180.0;
    point_t point;
    point.x = translatedX * cos(rotationAngle) - translatedY * sin(rotationAngle) + originX;
    point.y = translatedX * sin(rotationAngle) + translatedY * cos(rotationAngle) + originY;
    return point;
}

/**
 * @brief handleIRSensor
 * @param data
 * @param grid
 */
void handleIRSensor(SensorData_t data, Grid::GridType grid) {
    point_t distancePoint;
    bool maximum = false;
    if(data.distance == -2)
        return;
    if(data.distance == data.maximumMeasuringDistance) {
        distancePoint = data.maxSensorLocation;
        maximum = true;
    }
    else {
        distancePoint.x = data.sensorLocation.x + data.distance*cos(data.orientation*M_PI/180);
        distancePoint.y = data.sensorLocation.y + data.distance*sin(data.orientation*M_PI/180);
    }
    raytrace3(data.sensorLocation.x, data.sensorLocation.y, distancePoint.x, distancePoint.y, maximum, grid);
}

/**
 * @brief handleUltrasonicSensor
 *          For ultrasonic sensors we will only be using the distance measured to update the occupany grid with unoccupied cells.
 *          This will be done by decrementing the cells within the cone up to the distance point measured. By doing this we do
 *          not have to worry about knowing the exact location of the object whose distance is being measured within the sensor's cone.
 * @param data
 *          the ultrasonic sensor data
 * @param grid
 *          the occupany grid
 */
void handleUltrasonicSensor(SensorData_t data, Grid::GridType grid) {
    point_t distancePoint;
    bool maximum = true;
    // If we get negative 2 this indicates that the distance read was below the minimum measuring distance
    if(data.distance == -2)
        return;
    // If we get -1 this indicates that the distance read was further than the maximum measuring distance
    //      In this case we need to just set the distance point to the maximum distance point
    if(data.distance == data.maximumMeasuringDistance) {
        // Get the maximum distance point
        distancePoint = data.maxSensorLocation;
    }
    else {
        // If we get any other number then we update the occupany grid by decrementing all the cells within the cone up to the distance measured
        // Decrease the distance point so that we only decrement the cells up to the distance measured
        data.distance -= 2;
        distancePoint.x = data.sensorLocation.x + data.distance*cos(data.orientation*M_PI/180);
        distancePoint.y = data.sensorLocation.y + data.distance*sin(data.orientation*M_PI/180);
    }
    for(float angle = -data.coneAngle/2.0; angle < data.coneAngle/2.0; angle++) {
        //distancePoint = PointF(data.sensorLocation.x() + data.distance*cos((data.orientation + angle)*M_PI/180), data.sensorLocation.y() + data.distance*sin((data.orientation + angle)*M_PI/180));
        point_t rotatedDistancePoint = rotatePoint(data.sensorLocation.x, data.sensorLocation.y, distancePoint.x, distancePoint.y, angle);
        raytrace3(data.sensorLocation.x, data.sensorLocation.y, rotatedDistancePoint.x, rotatedDistancePoint.y, maximum, grid);
    }

}

/**
 * @brief updateOccupanyGrid
 * @param sensorData
 * @param grid
 */
void updateOccupanyGrid(SensorDataContainerType sensorData, Grid::GridType grid) {
    handleUltrasonicSensor(sensorData.leftFrontSensor, grid);
    handleUltrasonicSensor(sensorData.rightFrontSensor, grid);
    handleUltrasonicSensor(sensorData.middleFrontSensor, grid);
    handleUltrasonicSensor(sensorData.leftSideSensor, grid);
    handleUltrasonicSensor(sensorData.rightSideSensor, grid);
}

/*void updateOccupanyGrid2(SensorDataContainerType sensorData, Grid::GridType grid) {
    SensorData_t minSensor, nonMinSensor;
    int angleOffset;
    if(sensorData.leftFrontSensor.distance <= sensorData.rightFrontSensor.distance) {
        minSensor = sensorData.leftFrontSensor;
        nonMinSensor = sensorData.rightFrontSensor;
        angleOffset = -90;
    }
    else {
        minSensor = sensorData.rightFrontSensor;
        nonMinSensor = sensorData.leftFrontSensor;
        angleOffset = 90;
    }
    if(minSensor.distance > 0 && nonMinSensor.distance > 0) {
        minSensor.distance -= 2;
        int sensorDistanceBetween = 4;
        point_t minDistancePoint;
        minDistancePoint.x = minSensor.sensorLocation.x() + minSensor.distance*cos(minSensor.orientation*M_PI/180);
        minDistancePoint.y = minSensor.sensorLocation.y() + minSensor.distance*sin(minSensor.orientation*M_PI/180);
        float cosX = cos((minSensor.orientation + angleOffset)*M_PI/180);
        float sinY = sin((minSensor.orientation + angleOffset)*M_PI/180);
        // We are going to walk along the rover's front face from the sensor that returned the minimum distance to the other one
        for(int i = 0; i <= sensorDistanceBetween; i++) {
            point_t rayTracePoint(minDistancePoint.x() + i*cosX, minDistancePoint.y() + i*sinY);
            point_t  rayTraceOrigin(minSensor.sensorLocation.x() + i*cosX, minSensor.sensorLocation.y() + i*sinY);

            raytrace3(rayTraceOrigin.x(), rayTraceOrigin.y(), rayTracePoint.x(), rayTracePoint.y(), false, grid);
        }
    }

}*/

void updateOccupanyGrid3(SensorDataContainerType sensorData, Grid::GridType grid) {
    int i = 0;
//    if(sensorData.LongRangeIsValid == true) {
    {
        SensorData_t minSensor, nonMinSensor;
        int angleOffset;
        if(sensorData.leftFrontSensor.distance <= sensorData.rightFrontSensor.distance) {
            minSensor = sensorData.leftFrontSensor;
            nonMinSensor = sensorData.rightFrontSensor;
            angleOffset = -90;
        }
        else {
            minSensor = sensorData.rightFrontSensor;
            nonMinSensor = sensorData.leftFrontSensor;
            angleOffset = 90;
        }
        if(minSensor.distance > 0 && nonMinSensor.distance > 0) {
            bool maximum = minSensor.distance == 45.00;
            minSensor.distance -= 2;
            int sensorDistanceBetween = 4;
            point_t minDistancePoint;
            minDistancePoint.x = minSensor.sensorLocation.x + minSensor.distance*cos(minSensor.orientation*M_PI/180);
            minDistancePoint.y = minSensor.sensorLocation.y + minSensor.distance*sin(minSensor.orientation*M_PI/180);
            float cosX = cos((minSensor.orientation + angleOffset)*M_PI/180);
            float sinY = sin((minSensor.orientation + angleOffset)*M_PI/180);
            // We are going to walk along the rover's front face from the sensor that returned the minimum distance to the other one
            for(i = 0; i <= sensorDistanceBetween; i++) {
                point_t rayTracePoint;
                rayTracePoint.x = minDistancePoint.x + i*cosX;
                rayTracePoint.y = minDistancePoint.y + i*sinY;
                point_t rayTraceOrigin;
                rayTraceOrigin.x = minSensor.sensorLocation.x + i*cosX;
                rayTraceOrigin.y = minSensor.sensorLocation.y + i*sinY;

                raytrace3(rayTraceOrigin.x, rayTraceOrigin.y, rayTracePoint.x, rayTracePoint.y, maximum, grid);
            }
        }
//    }
    }
    {
//    if(sensorData.MidRangeIsValid == true) {
        SensorData_t minSensor, nonMinSensor;
        float minDistance;
        SensorData_t middleSensor = sensorData.middleFrontSensor;
        int angleOffset;
        if((sensorData.farLeftSensor.distance <= sensorData.farRightSensor.distance) && (sensorData.farLeftSensor.distance <= sensorData.middleFrontSensor.distance)) {
            minSensor = sensorData.farLeftSensor;
            nonMinSensor = sensorData.farRightSensor;
            minDistance = minSensor.distance;
            angleOffset = -90;

        }
        else if((sensorData.farRightSensor.distance <= sensorData.farLeftSensor.distance) && (sensorData.farRightSensor.distance <= sensorData.middleFrontSensor.distance)) {
            minSensor = sensorData.farRightSensor;
            nonMinSensor = sensorData.farLeftSensor;
            minDistance = minSensor.distance;
            angleOffset = 90;
        }
        else {
            minSensor = sensorData.farRightSensor;
            nonMinSensor = sensorData.farLeftSensor;
            minDistance = middleSensor.distance;
            angleOffset = 90;
        }
        if((minSensor.distance > 2) && (nonMinSensor.distance > 2) && (middleSensor.distance > 2)) {
            bool maximum = minDistance == 30.00;
            minDistance -= 2;
            int sensorDistanceBetween = 8;
            point_t minDistancePoint;
            minDistancePoint.x = minSensor.sensorLocation.x + (minDistance*cos(minSensor.orientation*M_PI/180));
            minDistancePoint.y = minSensor.sensorLocation.y + (minDistance*sin(minSensor.orientation*M_PI/180));
            float cosX = cos((minSensor.orientation + angleOffset)*M_PI/180);
            float sinY = sin((minSensor.orientation + angleOffset)*M_PI/180);
            for(i = 0; i <= sensorDistanceBetween; i++) {
                point_t rayTracePoint;
                rayTracePoint.x = minDistancePoint.x + (i*cosX);
                rayTracePoint.y = minDistancePoint.y + (i*sinY);
                point_t rayTraceOrigin;
                rayTraceOrigin.x = minSensor.sensorLocation.x + (i*cosX);
                rayTraceOrigin.y = minSensor.sensorLocation.y + (i*sinY);

                raytrace3(rayTraceOrigin.x, rayTraceOrigin.y, rayTracePoint.x, rayTracePoint.y, maximum, grid);
            }
        }
    }
//    }

}

float getDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
}

void interpretGrid(Grid::GridType grid, Grid::InterpretedInformationType* interpretedPoints) {
    if(interpretedPoints->number == 20) {
        return;
    }
    for(int y = 0; y < Grid::HEIGHT; y++) {
        for(int x = 0; x < Grid::WIDTH; x++) {
            if(interpretedPoints->number == 20) {
                return;
            }
            if(grid[y][x] >= 60) {
                if(interpretedPoints->number == 0) {
                    interpretedPoints->interpreted[interpretedPoints->number].x = x;
                    interpretedPoints->interpreted[interpretedPoints->number].y = y;
                    interpretedPoints->number++;
                }
                else {
                    for(int i = 0; i < interpretedPoints->number; i++) {
                        if(getDistance(x,y,interpretedPoints->interpreted[i].x, interpretedPoints->interpreted[i].y) > 10) {
                            interpretedPoints->interpreted[interpretedPoints->number].x = x;
                            interpretedPoints->interpreted[interpretedPoints->number].y = y;
                            interpretedPoints->number++;
                            break;
                        }
                    }
                }
            }
        }
    }
}


}
