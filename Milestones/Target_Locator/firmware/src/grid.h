#ifndef GRIDCELL_H
#define GRIDCELL_H
#include <stddef.h>
#include <stdbool.h>
#define WIDTH 160
#define HEIGHT 160
#define MAXIMUM 127
#define MINIMUM -128

typedef struct PointF {
    float x;
    float y;
} PointF;

typedef PointF point_t;

typedef point_t VertexType[20];
typedef struct {
    VertexType vertices;
    int number;
} VertexInformationType;

typedef point_t InterpretedType[20];
typedef struct {
    InterpretedType interpreted;
    int number;
} InterpretedInformationType;

typedef char GridType[HEIGHT][WIDTH];
typedef bool GridUpdatedType[HEIGHT];
void initializeGrid(GridType grid);
void incrementIndex(int x, int y, GridType grid);
void decrementIndex(int x, int y, GridType grid, GridUpdatedType updated);
void interpretGrid(GridType grid, InterpretedInformationType* interpretedPoints);

#endif // GRIDCELL_H
