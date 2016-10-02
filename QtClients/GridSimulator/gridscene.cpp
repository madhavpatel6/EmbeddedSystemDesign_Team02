#include "gridscene.h"

GridScene::GridScene(QWidget *parent) : QWidget(parent)
{
    this->resize(WIDTH*CELL_SIZE + 10, HEIGHT*CELL_SIZE + 10);
    this->setFixedSize(WIDTH*CELL_SIZE + 1, HEIGHT*CELL_SIZE + 1);
    initializeGrid();
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    roverLocation.location = QPoint(200, 200);
    roverLocation.orientation = 22;
}

GridScene::~GridScene() {

}

void GridScene::paintEvent(QPaintEvent *) {
    QPainter painter;
    painter.begin(this);
    QPen pen(Qt::white,1, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
//    qDebug() << "H " << this->size().height() <<" W " << this->size().width();
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            grid[i][j].draw(&painter);
        }
    }
    QPen pen1(Qt::black,2, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen1);
    for(int i = 0; i < lines.size(); i++) {
        painter.drawLine(lines[i]);
    }
    // xc and yc are the center of the widget's rect.
    qreal xc = roverLocation.location.x();
    qreal yc = roverLocation.location.y();

    painter.setPen(Qt::black);

    // draw the cross lines.
//    painter.drawLine(xc, rect().top(), xc, rect().bottom());
//    painter.drawLine(rect().left(), yc, rect().right(), yc);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::blue);

    // Draw a 13x17 rectangle rotated to 45 degrees around its center-point
    // in the center of the canvas.

    // translates the coordinate system by xc and yc
    painter.translate(xc, yc);

    // then rotate the coordinate system by 45 degrees
    painter.rotate(90 - roverLocation.orientation);

    // we need to move the rectangle that we draw by rx and ry so it's in the center.

    painter.drawRect(QRect(-6*CELL_SIZE/2, -6*CELL_SIZE/2, 6*CELL_SIZE, 6*CELL_SIZE));
    painter.setBrush(Qt::black);
    painter.drawRect(QRect(-1*CELL_SIZE/2, -6*CELL_SIZE/2, 1*CELL_SIZE, 1*CELL_SIZE));

    painter.end();
}


void GridScene::initializeGrid() {
    int locX = 0;
    int locY = 0;
    for(int i = 0; i < HEIGHT; i++) {
        std::vector<GridCell> tempV;
        for(int j = 0; j < WIDTH; j++) {
            GridCell tempCell = GridCell(locX,locY,CELL_SIZE);
            locX += CELL_SIZE;
            tempV.push_back(tempCell);
        }
        locX = 0;
        grid.push_back(tempV);
        locY += CELL_SIZE;
    }
}

void GridScene::addLine(float x1, float y1, float x2, float y2) {
    QLineF line = QLineF(x1, y1, x2, y2);
    raytrace2(x1, y1, x2, y2);
    lines.push_back(line);
}



void GridScene::raytrace(int x1, int y1, int const x2, int const y2)
{
    int delta_x(x2 - x1);
    qDebug() << "x = " << delta_x;
    // if x1 == x2, then it does not matter what we set here
    signed char const ix(((delta_x > 0) - (delta_x < 0))*CELL_SIZE);
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    qDebug() << "y = " << delta_y;
    // if y1 == y2, then it does not matter what we set here
    signed char const iy(((delta_y > 0) - (delta_y < 0))*CELL_SIZE);
    delta_y = std::abs(delta_y) << 1;

    //plot(x1, y1);

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));

        while (x1 != x2)
        {
            qDebug() << "x = " << y1/CELL_SIZE << "y = " << y1/CELL_SIZE;
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
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
        qDebug() << "Error = " << error;
        while (y1 != y2)
        {
            qDebug() << "x = " << y1/CELL_SIZE << "y = " << y1/CELL_SIZE;
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            grid[y1/CELL_SIZE][x1/CELL_SIZE].increment();
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing

            error += delta_x;
            y1 += iy;
            //plot(x1, y1);
        }
    }
}

void GridScene::raytrace2(double x0, double y0, double x1, double y1)
{
    double dx = fabs(x1 - x0);
    double dy = fabs(y1 - y0);
    x0 = x0/CELL_SIZE;
    y0 = y0/CELL_SIZE;
    x1 = x1/CELL_SIZE;
    y1 = y1/CELL_SIZE;
    qDebug() << "dx = " << dx << "dy = " << dy;
    int x = int(floor(x0));
    int y = int(floor(y0));

    double dt_dx = 1.0 / dx;
    double dt_dy = 1.0 / dy;
    qDebug() << "dt_dx = " << dt_dx << " dt_dy = " << dt_dy;
    double t = 0;

    int n = 1;
    int x_inc, y_inc;
    double t_next_vertical, t_next_horizontal;

    if (dx == 0)
    {
        x_inc = 0;
        t_next_horizontal = dt_dx * CELL_SIZE; // infinity
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
//    x_inc *= CELL_SIZE;
//    y_inc *= CELL_SIZE;
//    n = n / CELL_SIZE;
    qDebug() << "x_inc = " << x_inc << " y_inc = " << y_inc;
    for (; n > 0; --n)
    {
        grid[y][x].increment();
        grid[y][x].increment();
        grid[y][x].increment();
        grid[y][x].increment();
        grid[y][x].increment();
        grid[y][x].increment();
        //visit(x, y);
        qDebug() << "x = " << x << " y = " << y;
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
