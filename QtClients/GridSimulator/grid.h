#ifndef GRIDCELL_H
#define GRIDCELL_H

#include <QPainter>
#include <iostream>
#include "stdint.h"

namespace Grid {

    static const int WIDTH = 160;
    static const int HEIGHT = 160;
    static const int MAXIMUM = 127;
    static const int MINIMUM = -128;
    bool checkBounds(int x, int y);

    typedef int8_t GridType[HEIGHT][WIDTH];
    void initializeGrid(GridType grid);
    void incrementIndex(int x, int y, GridType grid);
    void decrementIndex(int x, int y, GridType grid);
    void draw(QPainter* painter, GridType grid, size_t size);
}

#endif // GRIDCELL_H
