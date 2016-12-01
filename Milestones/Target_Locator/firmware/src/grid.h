#ifndef GRIDCELL_H
#define GRIDCELL_H
#include <stddef.h>
#include <stdbool.h>
#define WIDTH 160
#define HEIGHT 160
#define MAXIMUM 127
#define MINIMUM -128

typedef char GridType[HEIGHT][WIDTH];
typedef bool GridUpdatedType[HEIGHT];
void initializeGrid(GridType grid);
void incrementIndex(int x, int y, GridType grid);
void decrementIndex(int x, int y, GridType grid, GridUpdatedType updated);


#endif // GRIDCELL_H
