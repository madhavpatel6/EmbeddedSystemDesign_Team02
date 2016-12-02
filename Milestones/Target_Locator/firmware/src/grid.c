#include "grid.h"
#include "math.h"
VertexInformationType vertices;
void initializeVertices(VertexInformationType ver) {
    vertices = ver;
}

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

bool isWithinArena(int x, int y) {
    point_t p;
    p.x = x;
    p.y = y;
    return PointInPolygon(p, vertices.number, vertices.vertices);
}

bool checkBounds(int x, int y) {
    return (y < HEIGHT) && (x < WIDTH) && (y >= 0) && (x >= 0);
}

void initializeGrid(GridType grid) {
    int locY = 0;
    int locX = 0;
    for(locY = 0; locY < HEIGHT; locY++) {
        for(locX = 0; locX < WIDTH; locX++) {
            grid[locY][locX] = MINIMUM;
        }
    }
}

void incrementIndex(int x, int y, GridType grid) {
    if(checkBounds(x,y)) {// && isWithinArena(x,y)) {
        if(grid[y][x] < (MAXIMUM - 32)) {
            grid[y][x]+=32;
        }
        if(grid[y][x] == 0) {
            grid[y][x] = 1;
        }
    }
}

void decrementIndex(int x, int y, GridType grid, GridUpdatedType updated) {
    if(checkBounds(x,y)) {// && isWithinArena(x,y)) {
        if(grid[y][x] > MINIMUM + 4) {
            grid[y][x]-= 4;
            updated[y] = true;
        }
        if(grid[y][x] == 0) {
            grid[y][x] = -1;
            updated[y] = true;
        }
    }
}

float getDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
}

void interpretGrid(GridType grid, InterpretedInformationType* interpretedPoints) {
    if(interpretedPoints->number == 20) {
        return;
    }
    int x = 0;
    int y = 0;
    int i = 0;
    for(y = 0; y < HEIGHT; y++) {
        for(x = 0; x < WIDTH; x++) {
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
                    for(i = 0; i < interpretedPoints->number; i++) {
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