#ifndef GRIDCELL_H
#define GRIDCELL_H
#include <stddef.h>
#include <stdbool.h>
#define WIDTH 50
#define HEIGHT 50
#define MAXIMUM 127
#define MINIMUM -128

bool checkBounds(int x, int y);

typedef char GridType[HEIGHT][WIDTH];
void initializeGrid(GridType grid);
void incrementIndex(int x, int y, GridType grid);
void decrementIndex(int x, int y, GridType grid);


#endif // GRIDCELL_H
