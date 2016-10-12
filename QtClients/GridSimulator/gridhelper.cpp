#include <iostream>
#include "gridscene.h"
#include "gridhelper.h"
namespace GridHelper {

void raytrace(double x1, double y1, double x2, double y2)
{
    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix(((delta_x > 0) - (delta_x < 0)));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy(((delta_y > 0) - (delta_y < 0)));
    delta_y = std::abs(delta_y) << 1;

    //plot(x1, y1);

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));

        while (floor(x1) != floor(x2))
        {
            if(GridScene::checkBounds(x1, y1)) {
                GridScene::grid[y1][x1].decrement();
            }
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing

            error += delta_y;
            x1 += ix;
            //plot(x1, y1);
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
        while (floor(y1) != floor(y2))
        {
            if(GridScene::checkBounds(x1, y1)) {
                GridScene::grid[y1][x1].decrement();
            }
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing

            error += delta_x;
            y1 += iy;
        }
    }
}

void raytrace2(double x0, double y0, double x1, double y1, bool maximum)
{
    double dx = fabs(x1 - x0);
    double dy = fabs(y1 - y0);
    x0 = x0;
    y0 = y0;
    x1 = x1;
    y1 = y1;
    int x = int(floor(x0));
    int y = int(floor(y0));

    double dt_dx = 1.0 / dx;
    double dt_dy = 1.0 / dy;
    double t = 0;

    int n = 1;
    int x_inc, y_inc;
    double t_next_vertical, t_next_horizontal;

    if (dx == 0)
    {
        x_inc = 0;
        t_next_horizontal = dt_dx; // infinity
    }
    else if (x1 > x0)
    {
        x_inc = 1;
        n += int(floor(x1)) - x;
        t_next_horizontal = (floor(x0) + 1 - x0) * dt_dx;
    }
    else
    {
        x_inc = -1;
        n += x - int(floor(x1));
        t_next_horizontal = (x0 - floor(x0)) * dt_dx;
    }

    if (dy == 0)
    {
        y_inc = 0;
        t_next_vertical = dt_dy; // infinity
    }
    else if (y1 > y0)
    {
        y_inc = 1;
        n += int(floor(y1)) - y;
        t_next_vertical = (floor(y0) + 1 - y0) * dt_dy;
    }
    else
    {
        y_inc = -1;
        n += y - int(floor(y1));
        t_next_vertical = (y0 - floor(y0)) * dt_dy;
    }
    for (; n > 0; --n)
    {
        if(GridScene::checkBounds(y, x)) {
            if(n == 1 && !maximum) {
                GridScene::grid[y][x].increment();
            }
            else{
                GridScene::grid[y][x].decrement();
            }
        }
        if (t_next_vertical < t_next_horizontal)
        {
            y += y_inc;
            t = t_next_vertical;
            t_next_vertical += dt_dy;
        }
        else
        {
            x += x_inc;
            t = t_next_horizontal;
            t_next_horizontal += dt_dx;
        }
    }
}

void raytrace3(int x1, int y1, int x2, int y2)
{
  int i;               // loop counter
  int ystep, xstep;    // the step on y and x axis
  int error;           // the error accumulated during the increment
  int errorprev;       // *vision the previous value of the error variable
  int y = y1, x = x1;  // the line points
  int ddy, ddx;        // compulsory variables: the double values of dy and dx
  int dx = x2 - x1;
  int dy = y2 - y1;
  if(y1 < GridScene::HEIGHT && x1 < GridScene::WIDTH) {
    GridScene::grid[y1][x1].decrement();// first point
  }
  // NB the last point can't be here, because of its previous point (which has to be verified)
  if (dy < 0){
    ystep = -1;
    dy = -dy;
  }else
    ystep = 1;
  if (dx < 0){
    xstep = -1;
    dx = -dx;
  }else
    xstep = 1;
  ddy = 2 * dy;  // work with double values for full precision
  ddx = 2 * dx;
  if (ddx >= ddy){  // first octant (0 <= slope <= 1)
    // compulsory initialization (even for errorprev, needed when dx==dy)
    errorprev = error = dx;  // start in the middle of the square
    for (i=0 ; i < dx ; i++){  // do not use the first point (already done)
      x += xstep;
      error += ddy;
      if (error > ddx){  // increment y if AFTER the middle ( > )
        y += ystep;
        error -= ddx;
        // three cases (octant == right->right-top for directions below):
        if(GridScene::checkBounds(x, y) && GridScene::checkBounds(x, y-ystep) && GridScene::checkBounds(x-xstep, y)) {
            if (error + errorprev < ddx)  // bottom square also
              GridScene::grid[y-ystep][x].decrement();
            else if (error + errorprev > ddx)  // left square also
              GridScene::grid[y][x-xstep].decrement();
            else {  // corner: bottom and left squares also
              GridScene::grid[y-ystep][x].decrement();
              GridScene::grid[y][x-xstep].decrement();
            }
        }
      }
      if(GridScene::checkBounds(x, y)) {
        GridScene::grid[y][x].decrement();
      }
      errorprev = error;
    }
  }else{  // the same as above
    errorprev = error = dy;
    for (i=0 ; i < dy ; i++){
      y += ystep;
      error += ddx;
      if (error > ddy){
        x += xstep;
        error -= ddy;
        if(GridScene::checkBounds(x, y) && GridScene::checkBounds(x, y-ystep) && GridScene::checkBounds(x-xstep, y)) {
            if (error + errorprev < ddy)
              GridScene::grid[y][x-xstep].decrement();
            else if (error + errorprev > ddy)
              GridScene::grid[y-ystep][x].decrement();
            else{
              GridScene::grid[y][x-xstep].decrement();
              GridScene::grid[y-ystep][x].decrement();
            }
        }
      }
      if(GridScene::checkBounds(x, y))
        GridScene::grid[y][x].decrement();
      errorprev = error;
    }
  }
  // assert ((y == y2) && (x == x2));  // the last point (y2,x2) has to be the same with the last point of the algorithm
}

void updateOccupanyGrid(SensorDataType sensorData) {

}
}
