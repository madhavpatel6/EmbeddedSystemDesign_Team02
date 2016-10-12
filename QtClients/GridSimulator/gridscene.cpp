#include "gridscene.h"
#include <math.h>
#include <QMouseEvent>
#define M_PI 3.14159265358979323846

GridScene::GridScene(QWidget *parent) : QWidget(parent)
{
    this->resize(WIDTH*CELL_SIZE + 10, HEIGHT*CELL_SIZE + 10);
    this->setFixedSize(WIDTH*CELL_SIZE + 1, HEIGHT*CELL_SIZE + 1);
    initializeGrid();
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    rover = new RoverClass();
    middleFrontSensor = new SensorClass(SensorClass::IRSENSOR, SensorClass::MIDDLESENSOR, 4, 70, 30, rover->getLocationInformation().center, rover->getLocationInformation().orientation, CELL_SIZE);
    leftFrontSensor = new SensorClass(SensorClass::IRSENSOR, SensorClass::LEFTSENSOR, 20, 70, 0, rover->getLocationInformation().center, rover->getLocationInformation().orientation, CELL_SIZE);
    rightFrontSensor = new SensorClass(SensorClass::IRSENSOR, SensorClass::RIGHTSENSOR, 20, 70, 0, rover->getLocationInformation().center, rover->getLocationInformation().orientation, CELL_SIZE);
    rightSideSensor = new SensorClass(SensorClass::ULTRASONICSENSOR, SensorClass::RIGHTSIDESENSOR, 4, 50, 30, rover->getLocationInformation().center, rover->getLocationInformation().orientation, CELL_SIZE);
    leftSideSensor = new SensorClass(SensorClass::ULTRASONICSENSOR, SensorClass::LEFTSIDESENSOR, 4, 50, 30, rover->getLocationInformation().center, rover->getLocationInformation().orientation, CELL_SIZE);
    this->setMouseTracking(true);
    mouseState = FIRSTCLICK;
    setFocusPolicy(Qt::StrongFocus);
    showObjects = true;
    newRectAngle = 0;
    newRect = QPolygonF();
    newRotatedRect = QPolygonF();
    newRectCenter = QPointF();
    mouseState = FIRSTCLICK;
}

GridScene::~GridScene() {
    delete middleFrontSensor;
    delete rightFrontSensor;
    delete leftFrontSensor;
}

void GridScene::reset() {
    for(int locY = 0; locY < HEIGHT; locY++) {
        for(int locX = 0; locX < WIDTH; locX++) {
            grid[locY][locX].reset();
        }
    }
}

void GridScene::paintEvent(QPaintEvent *) {
    QPainter painter;
    painter.begin(this);
    QPen pen(Qt::white,1, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
    painter.translate(0,height());
    painter.scale(1,-1);

    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            grid[i][j].draw(&painter);
        }
    }
    QPen pen1(Qt::black,2, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen1);

    painter.save();
    // xc and yc are the center of the widget's rect.
    qreal xc = rover->getLocationInformation().center.x() * CELL_SIZE;
    qreal yc = rover->getLocationInformation().center.y() * CELL_SIZE;

    painter.setBrush(Qt::white);
    painter.setPen(Qt::blue);

    // translates the coordinate system by xc and yc
    painter.translate(xc, yc);

    // then rotate the coordinate system by the number of degrees
    painter.rotate(rover->getLocationInformation().orientation);

    // we need to move the rectangle that we draw by rx and ry so it's in the center.

    painter.drawRect(QRect(-6*CELL_SIZE/2, -6*CELL_SIZE/2, 6*CELL_SIZE, 6*CELL_SIZE));
    painter.setBrush(Qt::black);


    painter.restore();
    painter.setPen(Qt::blue);
    painter.setBrush(Qt::blue);
    if(showObjects){
        for(int i = 0; i < rects.size(); i++)
            painter.drawPolygon(rects[i]);
    }

    if(mouseState != ROTATE) {
        painter.drawPolygon(newRect);
    }
    else {
        painter.drawPolygon(newRotatedRect);
    }
    middleFrontSensor->draw(&painter);
    leftFrontSensor->draw(&painter);
    rightFrontSensor->draw(&painter);
    rightSideSensor->draw(&painter);
    leftSideSensor->draw(&painter);
    painter.drawLines(lines);
    painter.end();
}

void GridScene::initializeGrid() {
    for(int locY = 0; locY < HEIGHT; locY++) {
        std::vector<GridCell> tempV;
        for(int locX = 0; locX < WIDTH; locX++) {
            GridCell tempCell = GridCell(locX,locY,CELL_SIZE);
            tempV.push_back(tempCell);
        }
        grid.push_back(tempV);
    }
}

void GridScene::addLine(double x1, double y1, double x2, double y2) {
//    raytrace3(floor(x1), floor(y1), floor(x2), floor(y2));
    raytrace2(x1, y1, x2, y2,false);
    QLineF line = QLineF(CELL_SIZE*x1, CELL_SIZE*y1, CELL_SIZE*x2, CELL_SIZE*y2);
    //Uncomment this if you want to see a line from a sensor to each reading
//    lines.push_back(line);
}

void GridScene::updateSensorReading() {
    addRayTrace(middleFrontSensor);
    addRayTrace(leftFrontSensor);
    addRayTrace(rightFrontSensor);
//    addRayTrace(rightSideSensor);
//    addRayTrace(leftSideSensor);
    update();
}

void GridScene::addRayTrace(SensorClass* impl) {
    float distance = impl->readDistance(rects);
    QPointF sensorLocation = impl->getSensorLocation();
    int sensorOrientation = impl->getSensorOrientation();
    QPointF distancePoint;
    bool maximum = false;
    if(distance == -2)
        return;
    if(distance == -1) {
        distancePoint = impl->getMaximumSensorLocation();
        maximum = true;
    }
    else {
        distancePoint = QPointF(sensorLocation.x() + distance*cos(sensorOrientation*M_PI/180), sensorLocation.y() + distance*sin(sensorOrientation*M_PI/180));
    }
    raytrace2(sensorLocation.x(), sensorLocation.y(), distancePoint.x(), distancePoint.y(), maximum);
}

GridScene::SensorDataType GridScene::getSensorData() {
    SensorDataType data;
    data.leftFrontSensorDistance = leftFrontSensor->readDistance(rects);
    data.middleFrontSensorDistance = middleFrontSensor->readDistance(rects);
    data.rightFrontSensorDistance = rightFrontSensor->readDistance(rects);
    data.leftFrontSensorDistance = leftFrontSensor->readDistance(rects);
    return data;
}

void GridScene::updateOccupanyGrid(SensorDataType sensorData) {

}

void GridScene::mouseMoveEvent(QMouseEvent *event) {
    QPointF pos = event->pos();
    pos.setY(CELL_SIZE*HEIGHT - pos.y());
    if(this->rect().contains(event->pos())) {
        emit updateCursorPosition(event->x(), height()-event->y());
        if(mouseState == SECONDCLICK) {
            QPointF bottomleft = QPointF(newRect[0].x(), pos.y());
            if(newRect.size() != 1) {
                newRect[1] = bottomleft;
                newRect[2] = pos;
                QPointF topright = QPointF(pos.x(), newRect[0].y());
                newRect[3] = topright;
            }
            else {
                newRect << bottomleft;
                newRect << pos;
                QPointF topright = QPointF(pos.x(), newRect[0].y());
                newRect << topright;
            }
        }
        else if(mouseState == ROTATE) {
            QPointF diffMouse = QPointF(pos.x() - newRectCenter.x(), pos.y() - newRectCenter.y());
            QPointF diffX = QPointF(5,0);
            float dot = diffMouse.x()*diffX.x() + diffMouse.y()*diffX.y();
            float det = diffMouse.x()*diffX.y() - diffMouse.y()*diffX.x();
            newRectAngle = -atan2(det, dot);
            QTransform t;
            t.translate(newRectCenter.x(), newRectCenter.y());
            t.rotate(newRectAngle*180/M_PI);
            t.translate(-newRectCenter.x(), -newRectCenter.y());
            newRotatedRect = t.map(newRect);
        }
    }
}


void GridScene::mouseReleaseEvent(QMouseEvent *event) {
    QPointF pos = event->pos();
    pos.setY(CELL_SIZE*HEIGHT - pos.y());
    switch(mouseState) {
        case FIRSTCLICK: {
            newRect << pos;
            mouseState = SECONDCLICK;
            break;
        }
        case SECONDCLICK: {
            QPointF bottomleft = QPointF(newRect[0].x(), pos.y());
            newRect[1] = bottomleft;
            newRect[2] = pos;
            QPointF topright = QPointF(pos.x(), newRect[0].y());
            newRect[3] = topright;

            newRectCenter = QPointF(newRect[0].x() + newRect[2].x(), newRect[0].y() + newRect[2].y());
            newRectCenter.setX(newRectCenter.x()/2.0);
            newRectCenter.setY(newRectCenter.y()/2.0);
            mouseState = ROTATE;
            update();
            break;
        }
        case ROTATE: {
            QPointF diffMouse = QPointF(pos.x() - newRectCenter.x(), pos.y() - newRectCenter.y());
            QPointF diffX = QPointF(5,0);
            float dot = diffMouse.x()*diffX.x() + diffMouse.y()*diffX.y();
            float det = diffMouse.x()*diffX.y() - diffMouse.y()*diffX.x();
            newRectAngle = -atan2(det, dot);
            QTransform t;
            t.translate(newRectCenter.x(), newRectCenter.y());
            t.rotate(newRectAngle*180/M_PI);
            t.translate(-newRectCenter.x(), -newRectCenter.y());
            newRotatedRect = t.map(newRect);
            rects.push_back(newRotatedRect);
            newRectAngle = 0;
            newRect = QPolygonF();
            newRotatedRect = QPolygonF();
            newRectCenter = QPointF();
            mouseState = FIRSTCLICK;
            update();
            break;
        }
    }
}

void GridScene::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Left: case Qt::Key_A:{
        rover->turnRoverLeft(2);
        break;
    }
    case Qt::Key_Up: case Qt::Key_W:{
        rover->moveRoverUp(1.5);
        break;
    }
    case Qt::Key_Down: case Qt::Key_S:{
        rover->moveRoverBack(1.5);
        break;
    }
    case Qt::Key_Right: case Qt::Key_D:{
        rover->turnRoverRight(2);
        break;
    }
    default:
        break;
    }
    emit updateRoverPosition(rover->getLocationInformation().center.x(), rover->getLocationInformation().center.y(), rover->getLocationInformation().orientation);
    middleFrontSensor->updatePosition(rover->getLocationInformation().center, rover->getLocationInformation().orientation);
    leftFrontSensor->updatePosition(rover->getLocationInformation().center, rover->getLocationInformation().orientation);
    rightFrontSensor->updatePosition(rover->getLocationInformation().center, rover->getLocationInformation().orientation);
    rightSideSensor->updatePosition(rover->getLocationInformation().center, rover->getLocationInformation().orientation);
    leftSideSensor->updatePosition(rover->getLocationInformation().center, rover->getLocationInformation().orientation);
    this->update();
}

QPointF GridScene::rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle) {
    int translatedX = pointX - originX;
    int translatedY = pointY - originY;
    rotationAngle = rotationAngle*M_PI/180.0;
    QPointF point = QPointF(translatedX * cos(rotationAngle) - translatedY * sin(rotationAngle) + originX,
                  translatedX * sin(rotationAngle) + translatedY * cos(rotationAngle) + originY);
    return point;
}

void GridScene::raytrace(double x1, double y1, double x2, double y2)
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
            if(checkBounds(x1, y1)) {
                grid[y1][x1].decrement();
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
            if(checkBounds(x1, y1)) {
                grid[y1][x1].decrement();
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

void GridScene::raytrace2(double x0, double y0, double x1, double y1, bool maximum)
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
        if(checkBounds(y, x)) {
            if(n == 1 && !maximum) {
                grid[y][x].increment();
            }
            else{
                grid[y][x].decrement();
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

void GridScene::raytrace3(int x1, int y1, int x2, int y2)
{
  int i;               // loop counter
  int ystep, xstep;    // the step on y and x axis
  int error;           // the error accumulated during the increment
  int errorprev;       // *vision the previous value of the error variable
  int y = y1, x = x1;  // the line points
  int ddy, ddx;        // compulsory variables: the double values of dy and dx
  int dx = x2 - x1;
  int dy = y2 - y1;
  if(y1 < HEIGHT && x1 < WIDTH) {
    grid[y1][x1].decrement();// first point
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
        if(checkBounds(x, y) && checkBounds(x, y-ystep) && checkBounds(x-xstep, y)) {
            if (error + errorprev < ddx)  // bottom square also
              grid[y-ystep][x].decrement();
            else if (error + errorprev > ddx)  // left square also
              grid[y][x-xstep].decrement();
            else {  // corner: bottom and left squares also
              grid[y-ystep][x].decrement();
              grid[y][x-xstep].decrement();
            }
        }
      }
      if(checkBounds(x, y)) {
        grid[y][x].decrement();
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
        if(checkBounds(x, y) && checkBounds(x, y-ystep) && checkBounds(x-xstep, y)) {
            if (error + errorprev < ddy)
              grid[y][x-xstep].decrement();
            else if (error + errorprev > ddy)
              grid[y-ystep][x].decrement();
            else{
              grid[y][x-xstep].decrement();
              grid[y-ystep][x].decrement();
            }
        }
      }
      if(checkBounds(x, y))
        grid[y][x].decrement();
      errorprev = error;
    }
  }
  // assert ((y == y2) && (x == x2));  // the last point (y2,x2) has to be the same with the last point of the algorithm
}
