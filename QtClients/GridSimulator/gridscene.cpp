#include "gridscene.h"
#include <math.h>
#include <QMouseEvent>
#include "gridhelper.h"
#define M_PI 3.14159265358979323846

GridScene::GridScene(QWidget *parent) : QWidget(parent)
{
    this->resize(Grid::WIDTH*CELL_SIZE + 10, Grid::HEIGHT*CELL_SIZE + 10);
    this->setFixedSize(Grid::WIDTH*CELL_SIZE + 1, Grid::HEIGHT*CELL_SIZE + 1);
    Grid::initializeGrid(grid);
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
    Grid::initializeGrid(grid);
}

void GridScene::paintEvent(QPaintEvent *) {
    QPainter painter;
    painter.begin(this);
    QPen pen(Qt::white,1, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
    painter.translate(0,height());
    painter.scale(1,-1);

    Grid::draw(&painter, grid, CELL_SIZE);
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

void GridScene::addLine(double x1, double y1, double x2, double y2) {
//    raytrace3(floor(x1), floor(y1), floor(x2), floor(y2));
    GridHelper::raytrace2(x1, y1, x2, y2, false, grid);
    QLineF line = QLineF(CELL_SIZE*x1, CELL_SIZE*y1, CELL_SIZE*x2, CELL_SIZE*y2);
    //Uncomment this if you want to see a line from a sensor to each reading
//    lines.push_back(line);
}

void GridScene::updateSensorReading() {
//    SensorDataType data = getSensorData();
//    GridHelper::updateOccupanyGrid(data);
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
    GridHelper::raytrace3(sensorLocation.x(), sensorLocation.y(), distancePoint.x(), distancePoint.y(), maximum, grid);
}

SensorDataType GridScene::getSensorData() {
    SensorDataType data;
    data.leftFrontSensor = leftFrontSensor->getParamAndDistance(rects);
    data.middleFrontSensor = middleFrontSensor->getParamAndDistance(rects);
    data.rightFrontSensor = rightFrontSensor->getParamAndDistance(rects);
    data.leftFrontSensor = leftFrontSensor->getParamAndDistance(rects);
    return data;
}


void GridScene::mouseMoveEvent(QMouseEvent *event) {
    QPointF pos = event->pos();
    pos.setY(CELL_SIZE*Grid::HEIGHT - pos.y());
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
    pos.setY(CELL_SIZE*Grid::HEIGHT - pos.y());
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

