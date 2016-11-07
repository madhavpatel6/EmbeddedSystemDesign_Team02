#ifndef GRIDCELL_H
#define GRIDCELL_H
#include <stddef.h>
#include <stdbool.h>
#define WIDTH 200
#define HEIGHT 200
#define MAXIMUM 127
#define MINIMUM -128

typedef char GridType[HEIGHT][WIDTH];
void initializeGrid(GridType grid);
void incrementIndex(int x, int y, GridType grid);
void decrementIndex(int x, int y, GridType grid);


#endif // GRIDCELL_H
