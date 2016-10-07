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
    middleFrontSensor = new SensorClass(roverLocation.center, roverLocation.orientation, 30, CELL_SIZE, SensorClass::MIDDLESENSOR);
    leftFrontSensor = new SensorClass(roverLocation.center, roverLocation.orientation, 30, CELL_SIZE, SensorClass::LEFTSENSOR);
    rightFrontSensor = new SensorClass(roverLocation.center, roverLocation.orientation, 30, CELL_SIZE, SensorClass::RIGHTSENSOR);

    updateRoverLocation(QPoint(50,50), 0);

    this->setMouseTracking(true);
    mouseState = FIRSTCORNER;
    setFocusPolicy(Qt::StrongFocus);
    showObjects = true;
}

GridScene::~GridScene() {
    delete middleFrontSensor;
    delete rightFrontSensor;
    delete leftFrontSensor;

}

void GridScene::updateRoverLocation(QPoint center, int angle) {
    roverLocation.center = center;
    roverLocation.orientation = angle;
    middleFrontSensor->updatePosition(roverLocation.center, roverLocation.orientation);
    leftFrontSensor->updatePosition(roverLocation.center, roverLocation.orientation);
    rightFrontSensor->updatePosition(roverLocation.center, roverLocation.orientation);
    this->update();
}

void GridScene::paintEvent(QPaintEvent *) {
    QPainter painter;
    painter.begin(this);
    QPen pen(Qt::white,1, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
//    painter.translate(width()2, height()/2);
    painter.translate(0,height());
    painter.scale(1,-1);
//    painter.drawEllipse(QPoint(5,5),5,5);

//    qDebug() << "H " << this->size().height() <<" W " << this->size().width();
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            grid[i][j].draw(&painter);
        }
    }
    QPen pen1(Qt::black,2, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen1);
//    for(int i = 0; i < lines.size(); i++) {
//        painter.drawLine(lines[i]);
//    }
    painter.save();
    // xc and yc are the center of the widget's rect.
    qreal xc = roverLocation.center.x() * CELL_SIZE;
    qreal yc = roverLocation.center.y() * CELL_SIZE;

    painter.setBrush(Qt::white);
    painter.setPen(Qt::blue);

    // Draw a 13x17 rectangle rotated to 45 degrees around its center-point
    // in the center of the canvas.

    // translates the coordinate system by xc and yc
    painter.translate(xc, yc);

    // then rotate the coordinate system by the number of degrees
    painter.rotate(roverLocation.orientation);

    // we need to move the rectangle that we draw by rx and ry so it's in the center.

    painter.drawRect(QRect(-6*CELL_SIZE/2, -6*CELL_SIZE/2, 6*CELL_SIZE, 6*CELL_SIZE));
    painter.setBrush(Qt::black);


    painter.restore();
    painter.setPen(Qt::blue);
    painter.setBrush(Qt::blue);
    if(showObjects) painter.drawRects(rects);

    painter.drawRect(newRect);
    middleFrontSensor->draw(&painter);
    leftFrontSensor->draw(&painter);
    rightFrontSensor->draw(&painter);
//    painter.drawLines(lin);
    painter.end();
}

void GridScene::initializeGrid() {
    for(int locY = 0; locY < HEIGHT; locY++) {
        std::vector<GridCell> tempV;
        for(int locX = 0; locX < WIDTH; locX++) {
            GridCell tempCell = GridCell(locX,locY,CELL_SIZE);
//            qDebug() << "X loc " << locX << " Y loc " << locY;
            tempV.push_back(tempCell);
        }
//        qDebug() << "locY " << locY;
        grid.push_back(tempV);
    }
}

void GridScene::addLine(float x1, float y1, float x2, float y2) {
    QLineF line = QLineF(x1, y1, x2, y2);
    //Uncomment this if you want to see a line from a sensor to each reading
    lines.push_back(line);
}

void GridScene::updateSensorReading(SensorLocation asds, int asdf) {
    int distanceMiddle = middleFrontSensor->readDistance(rects);
    qDebug() << "distance = " << distanceMiddle;
    QPoint middleSensorLocation = middleFrontSensor->getSensorLocation();
    int middleSensorOrientation = middleFrontSensor->getSensorOrientation();
//    qDebug() << "distance  = " << distanceMiddle;
    QPoint middleDistancePoint;
    bool maximum = false;
    if(distanceMiddle != -1) {
        middleDistancePoint = QPoint(middleSensorLocation.x() + distanceMiddle*cos(middleSensorOrientation*M_PI/180), middleSensorLocation.y() + distanceMiddle*sin(middleSensorOrientation*M_PI/180));
    }
    else {
        middleDistancePoint = middleFrontSensor->getMaximumSensorLocation();
        maximum = true;
    }
//    qDebug() << "Sensor Location  = " << middleSensorLocation << " distance location = " << middleDistancePoint << " sensor orientation = " << middleSensorOrientation;
//    addLine(middleSensorLocation.x(), middleSensorLocation.y(), middleDistancePoint.x(), middleDistancePoint.y());
    raytrace2(middleSensorLocation.x(), middleSensorLocation.y(), middleDistancePoint.x(), middleDistancePoint.y(), maximum);
    update();
}

void GridScene::mouseMoveEvent(QMouseEvent *_event) {
    if(this->rect().contains(_event->pos())) {
        emit updateCursorPosition(_event->x(), _event->y());
    }
}

void GridScene::mousePressEvent(QMouseEvent *event) {
    if(event->buttons() == Qt::LeftButton) {
        QPoint pos = event->pos();
        pos.setY(CELL_SIZE*HEIGHT - pos.y());
        newRect.setTopLeft(pos);
        newRect.setBottomRight(pos);
//        qDebug() << "Pressed at " << pos;
    }
}

void GridScene::mouseReleaseEvent(QMouseEvent *event) {
    QPoint pos = event->pos();
//    qDebug() << pos;
//    pos.setX(pos.x() - width()/2);
    pos.setY(CELL_SIZE*HEIGHT - pos.y());
    newRect.setBottomRight(pos);
    rects.push_back(newRect);
    qDebug() << "new object created at " << newRect;
//    qDebug() << newRect.contains(100,100,false);
    newRect = QRect();
//    newRect = QRect(QPoint(0,0), QPoint(200,200));
//    qDebug() << "Released at " << pos;
    update();
}

void GridScene::keyPressEvent(QKeyEvent *event) {
//    qDebug() << "Key Press Event";
//    qDebug() << event->key();
//    qDebug() << Qt::DownArrow;
    switch(event->key()) {
    case Qt::Key_Left: case Qt::Key_A:{
        turnRoverLeft(2);
        break;
    }
    case Qt::Key_Up: case Qt::Key_W:{
//        qDebug() << "Moving Up";
        moveRoverUp(2);
        break;
    }
    case Qt::Key_Down: case Qt::Key_S:{
//        qDebug() << "Moving Back";
        moveRoverBack(2);
        break;
    }
    case Qt::Key_Right: case Qt::Key_D:{
        turnRoverRight(2);
        break;
    }
    default:
        break;
    }
    update();
}

void GridScene::moveRoverUp(int distanceCM) {
//    int pixelDistance = distanceCM*CELL_SIZE;
//    qDebug() << "Pixel distance " << pixelDistance;
    QPoint newPosition = QPoint(roverLocation.center.x() + cos(roverLocation.orientation*M_PI/180.0) * distanceCM,
                                roverLocation.center.y() + sin(roverLocation.orientation*M_PI/180.0) * distanceCM);
//    qDebug() << "new position " << newPosition;
    updateRoverLocation(newPosition, roverLocation.orientation);
}

void GridScene::moveRoverBack(int distanceCM) {
//    int pixelDistance = distanceCM*CELL_SIZE;
    QPoint newPosition = QPoint(roverLocation.center.x() - cos(roverLocation.orientation*M_PI/180.0) * distanceCM,
                                roverLocation.center.y() - sin(roverLocation.orientation*M_PI/180.0) * distanceCM);
    updateRoverLocation(newPosition, roverLocation.orientation);
}

void GridScene::turnRoverRight(int angleDegrees) {
    updateRoverLocation(roverLocation.center, roverLocation.orientation - angleDegrees);
}

void GridScene::turnRoverLeft(int angleDegrees) {
    updateRoverLocation(roverLocation.center, roverLocation.orientation + angleDegrees);
}

QPoint GridScene::rotatePoint(int originX, int originY, int pointX, int pointY, double rotationAngle) {
    int translatedX = pointX - originX;
    int translatedY = pointY - originY;
    rotationAngle = rotationAngle*M_PI/180.0;
//    qDebug() << "translated X = " << translatedX << " translatedY = " << translatedY;
    QPoint point = QPoint(translatedX * cos(rotationAngle) - translatedY * sin(rotationAngle) + originX,
                  translatedX * sin(rotationAngle) + translatedY * cos(rotationAngle) + originY);
    return point;
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

void GridScene::raytrace2(double x0, double y0, double x1, double y1, bool maximum)
{
//    qDebug() << " first point x = " << x0 << " y = " << y0 << " second point x = " << x1 << " y = " << y1;
    double dx = fabs(x1 - x0);
    double dy = fabs(y1 - y0);
    x0 = x0;
    y0 = y0;
    x1 = x1;
    y1 = y1;
//    qDebug() << "dx = " << dx << "dy = " << dy;
    int x = int(floor(x0));
    int y = int(floor(y0));

    double dt_dx = 1.0 / dx;
    double dt_dy = 1.0 / dy;
//    qDebug() << "dt_dx = " << dt_dx << " dt_dy = " << dt_dy;
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
//    x_inc *= CELL_SIZE;
//    y_inc *= CELL_SIZE;
//    n = n / CELL_SIZE;
//    qDebug() << "xinc = " << x_inc;
//    qDebug() << "yinc = " << y_inc;
//    qDebug() << "n = " << n;
//    qDebug() << "x_inc = " << x_inc << " y_inc = " << y_inc;
    for (; n > 0; --n)
    {
//        qDebug() << "X = " << x << " Y = " << y;
        if(n == 1 && !maximum) {
//            qDebug() << maximum;
            grid[y][x].increment();
            grid[y][x].increment();
            grid[y][x].increment();
        }
        else {
//            qDebug() << "decrementing";
            grid[y][x].decrement();
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

QPoint GridScene::findFirstIntersection(SensorLocation sensor) {
    switch(sensor) {
    case MIDDLESENSOR: {
//        middleFrontSensor->readDistance(rects);
        break;
    }
    case RIGHTSENSOR: {
//        rightFrontSensor->readDistance(rects);
        break;
    }
    case LEFTSENSOR: {
//        leftFrontSensor->readDistance(rects);
        break;
    }
    }
    return QPoint();
}
