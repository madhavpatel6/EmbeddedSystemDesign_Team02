#include "grid.h"

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
    if(checkBounds(x,y)) {
        if(grid[y][x] < (MAXIMUM - 32)) {
            grid[y][x]+=32;
        }
        if(grid[y][x] == 0) {
            grid[y][x] = 1;
        }
    }
}

void decrementIndex(int x, int y, GridType grid, GridUpdatedType updated) {
    if(checkBounds(x,y)) {
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