#include "polygon.h"

#define INF 10000

bool PointInPolygon(point_t point, int n, point_t points[]) {
    int i, j, nvert = n;
    bool c = false;

    for(i = 0, j = nvert - 1; i < nvert; j = i++) {
    if( ( (points[i].y >= point.y ) != (points[j].y >= point.y) ) &&
        (point.x <= (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
      )
      c = !c;
    }

    return c;
}
