#include "gridhelper.h"
#include "math.h"
#define M_PI 3.14159265358979323846

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
void raytrace3(int x1, int y1, int x2, int y2, bool maximum, GridType grid, GridUpdatedType updated)
{
    int i;               // loop counter
    int ystep, xstep;    // the step on y and x axis
    int error;           // the error accumulated during the increment
    int errorprev;       // *vision the previous value of the error variable
    int y = y1, x = x1;  // the line points
    int ddy, ddx;        // compulsory variables: the double values of dy and dx
    int dx = x2 - x1;
    int dy = y2 - y1;
    decrementIndex(x, y, grid, updated);
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
            if (error + errorprev < ddx) {  // bottom square als
                decrementIndex(x, y, grid, updated);
            }
            else if (error + errorprev > ddx) { // left square also
                decrementIndex(x-xstep, y, grid, updated);
            }
            else {  // corner: bottom and left squares also
                decrementIndex(x, y-ystep, grid, updated);
                decrementIndex(x-xstep, y, grid, updated);
            }
          }
            if(!maximum && i+1 == dx) {
                  incrementIndex(x, y, grid);
            }
            else {
                  decrementIndex(x, y, grid, updated);
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
                if (error + errorprev < ddy) {
                    decrementIndex(x-xstep,y,grid, updated);
                }
                else if (error + errorprev > ddy) {
                    decrementIndex(x, y-ystep, grid, updated);
                }
                else {
                    decrementIndex(x-xstep,y,grid, updated);
                    decrementIndex(x, y-ystep, grid, updated);
                }
            }
            if(!maximum && i+1 == dy) {
                incrementIndex(x, y, grid);
            }
            else {
                decrementIndex(x,y,grid, updated);
            }
            errorprev = error;
        }
    }
  // assert ((y == y2) && (x == x2));  // the last point (y2,x2) has to be the same with the last point of the algorithm
}

void rotatePoint(point_t* point, float originX, float originY, float pointX, float pointY, double rotationAngle) {
    float translatedX = pointX - originX;
    float translatedY = pointY - originY;
    float rot = rotationAngle*M_PI/180.0;
    point->x = translatedX * cos(rot) - translatedY * sin(rot) + originX;
    point->y = translatedX * sin(rot) + translatedY * cos(rot) + originY;
}

void updateOccupanyGrid2(SensorDataContainerType sensorData, GridType grid) {
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
        minDistancePoint.x = minSensor.sensorLocation.x + minSensor.distance*cos(minSensor.orientation*M_PI/180);
        minDistancePoint.y = minSensor.sensorLocation.y + minSensor.distance*sin(minSensor.orientation*M_PI/180);
        float cosX = cos((minSensor.orientation + angleOffset)*M_PI/180);
        float sinY = sin((minSensor.orientation + angleOffset)*M_PI/180);
        // We are going to walk along the rover's front face from the sensor that returned the minimum distance to the other one
        int i = 0;
        for(i = 0; i <= sensorDistanceBetween; i++) {
            point_t rayTracePoint;
            rayTracePoint.x = minDistancePoint.x + i*cosX;
            rayTracePoint.y = minDistancePoint.y + i*sinY;
            point_t rayTraceOrigin;
            rayTraceOrigin.x = minSensor.sensorLocation.x + i*cosX;
            rayTraceOrigin.y = minSensor.sensorLocation.y + i*sinY;

//            raytrace3(rayTraceOrigin.x, rayTraceOrigin.y, rayTracePoint.x, rayTracePoint.y, true, grid);
        }
    }
}

void updateOccupanyGrid3(SensorDataContainerType sensorData, GridType grid, GridUpdatedType updated) {
    int i = 0;
    if(sensorData.LongRangeIsValid == true) {
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
                
                raytrace3(rayTraceOrigin.x, rayTraceOrigin.y, rayTracePoint.x, rayTracePoint.y, maximum, grid, updated);
            }
        }
    }
    if(sensorData.MidRangeIsValid == true) {
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

                raytrace3(rayTraceOrigin.x, rayTraceOrigin.y, rayTracePoint.x, rayTracePoint.y, maximum, grid, updated);
            }
        }
    }

}


