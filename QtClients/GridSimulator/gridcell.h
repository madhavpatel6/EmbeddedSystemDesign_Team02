#ifndef GRIDCELL_H
#define GRIDCELL_H

#include <QPainter>
class GridCell
{
public:
    GridCell(int _x, int _y, int size);
    void draw(QPainter* painter);
    void increment();
    void decrement();
    void reset();
private:
    QRect box;
    int count;
    static const int MAXIMUM = 127;
    static const int MINIMUM = -128;
//    typedef enum { EMPTY, UNKNOWN, OCCUPIED } CellStateType;
//    CellStateType state;
};

#endif // GRIDCELL_H
