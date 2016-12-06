#ifndef GRIDCELL_H
#define GRIDCELL_H

#include <QPainter>
#include <iostream>
#include "stdint.h"
#include "grid.h"
typedef struct PointF {
    float x;
    float y;
} PointF;

typedef PointF point_t;

namespace Grid {
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
    static const int WIDTH = 160;
    static const int HEIGHT = 160;
    static const int MAXIMUM = 127;
    static const int MINIMUM = -128;
    bool checkBounds(int x, int y);

    void initializeVertices();
    typedef char GridType[HEIGHT][WIDTH];
    void initializeGrid(GridType grid);
    void incrementIndex(int x, int y, GridType grid);
    void decrementIndex(int x, int y, GridType grid);
    void draw(QPainter* painter, GridType grid, size_t size);
}

#endif // GRIDCELL_H
