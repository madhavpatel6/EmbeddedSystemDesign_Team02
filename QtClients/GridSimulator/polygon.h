#ifndef POLYGON_H
#define POLYGON_H

#include "gridhelper.h"

bool isInside(point_t polygon[], int n, point_t p);

bool PointInPolygon(point_t point, int n, point_t points[]);

#endif // POLYGON_H
