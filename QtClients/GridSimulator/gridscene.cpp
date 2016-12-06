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
    middleFrontSensor = new SensorClass(SensorClass::IRSENSOR, SensorClass::MIDDLESENSOR, 7, 30, 0, 0,rover->getEstimatedLocationInformation().center, rover->getEstimatedLocationInformation().orientation, CELL_SIZE);
    leftFrontSensor = new SensorClass(SensorClass::IRSENSOR, SensorClass::LEFTSENSOR, 20, 70, 0, 0, rover->getEstimatedLocationInformation().center, rover->getEstimatedLocationInformation().orientation, CELL_SIZE);
    rightFrontSensor = new SensorClass(SensorClass::IRSENSOR, SensorClass::RIGHTSENSOR, 20, 70, 0, 0, rover->getEstimatedLocationInformation().center, rover->getEstimatedLocationInformation().orientation, CELL_SIZE);
    farLeftSensor = new SensorClass(SensorClass::IRSENSOR, SensorClass::FARLEFTSENSOR, 7, 30, 0, 0, rover->getEstimatedLocationInformation().center, rover->getEstimatedLocationInformation().orientation, CELL_SIZE);
    farRightSensor = new SensorClass(SensorClass::IRSENSOR, SensorClass::FARRIGHTSENSOR, 7, 30, 0, 0, rover->getEstimatedLocationInformation().center, rover->getEstimatedLocationInformation().orientation, CELL_SIZE);

//    rightSideSensor = new SensorClass(SensorClass::ULTRASONICSENSOR, SensorClass::RIGHTSIDESENSOR, 2, 2, 0, 0, rover->getEstimatedLocationInformation().center, rover->getEstimatedLocationInformation().orientation, CELL_SIZE);
//    leftSideSensor = new SensorClass(SensorClass::ULTRASONICSENSOR, SensorClass::LEFTSIDESENSOR, 2, 2, 0, 0, rover->getEstimatedLocationInformation().center, rover->getEstimatedLocationInformation().orientation, CELL_SIZE);
    this->setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    showObjects = true;
    newRectAngle = 0;
    newRect = QPolygonF();
    newRotatedRect = QPolygonF();
    newRectCenter = QPointF();
    mouseState = FIRSTCLICK;
    Grid::initializeVertices();
    recordKeys = true;
}

GridScene::~GridScene() {
    delete middleFrontSensor;
    delete rightFrontSensor;
    delete leftFrontSensor;
}

void GridScene::resetGrid() {
    Grid::initializeGrid(grid);
}

void GridScene::resetRoverPosition() {
    rover->resetPosition();
    emit updateRoverPosition(rover->getEstimatedLocationInformation().center.x(), rover->getEstimatedLocationInformation().center.y(), rover->getEstimatedLocationInformation().orientation);
    middleFrontSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
    leftFrontSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
    rightFrontSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
//    rightSideSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
//    leftSideSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
    farLeftSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
    farRightSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
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

//    if(showObjects){
        rover->draw(&painter, CELL_SIZE);
//    }
    painter.setPen(Qt::blue);
    painter.setBrush(Qt::blue);
    if(showObjects){
        for(int i = 0; i < objects.size(); i++)
            painter.drawPolygon(objects[i]);
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
//    rightSideSensor->draw(&painter);
//    leftSideSensor->draw(&painter);
    farLeftSensor->draw(&painter);
    farRightSensor->draw(&painter);
    Grid::InterpretedInformationType intr;
    intr.number = 0;
    GridHelper::interpretGrid(grid, &intr);
    painter.setPen(QPen(QBrush(Qt::red),5));
    for(int i = 0; i < intr.number; i++) {
        painter.drawEllipse(intr.interpreted[i].x*CELL_SIZE, intr.interpreted[i].y*CELL_SIZE, 10, 10);
    }
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
    SensorDataContainerType data = getSensorData();
//    GridHelper::updateOccupanyGrid3(data, grid);
    addRayTrace(middleFrontSensor);
    addRayTrace(leftFrontSensor);
    addRayTrace(rightFrontSensor);
    addRayTrace(farLeftSensor);
    addRayTrace(farRightSensor);
    update();
}

void GridScene::addRayTrace(SensorClass* impl) {
    float distance = impl->readDistance(objects);
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
    GridHelper::raytrace3(sensorLocation.x(), sensorLocation.y(), distancePoint.x(), distancePoint.y(), false, grid);
}

SensorDataContainerType GridScene::getSensorData() {
    SensorDataContainerType data;
    data.leftFrontSensor = leftFrontSensor->getParamAndDistance(objects);
    data.middleFrontSensor = middleFrontSensor->getParamAndDistance(objects);
    data.rightFrontSensor = rightFrontSensor->getParamAndDistance(objects);
    data.farLeftSensor = farLeftSensor->getParamAndDistance(objects);
    data.farRightSensor = farRightSensor->getParamAndDistance(objects);
//    data.leftSideSensor = leftSideSensor->getParamAndDistance(objects);
//    data.rightSideSensor = rightSideSensor->getParamAndDistance(objects);

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
            objects.push_back(newRotatedRect);
            qDebug() << "New Rectangle " << newRotatedRect;
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
        if(recordKeys) keys.push_back(Qt::Key_A);
        rover->turnRoverLeft(2);
        break;
    }
    case Qt::Key_Up: case Qt::Key_W:{
        if(recordKeys) keys.push_back(Qt::Key_W);
        rover->moveRoverUp(1.5);
        break;
    }
    case Qt::Key_Down: case Qt::Key_S:{
        if(recordKeys) keys.push_back(Qt::Key_S);
        rover->moveRoverBack(1.5);
        break;
    }
    case Qt::Key_Right: case Qt::Key_D:{
        if(recordKeys) keys.push_back(Qt::Key_D);
        rover->turnRoverRight(2);
        break;
    }
    default:
        break;
    }
    emit updateRoverPosition(rover->getEstimatedLocationInformation().center.x(), rover->getEstimatedLocationInformation().center.y(), rover->getEstimatedLocationInformation().orientation);
    middleFrontSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
    leftFrontSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
    rightFrontSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
//    rightSideSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
//    leftSideSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
    farLeftSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
    farRightSensor->updatePosition(rover->getRealLocationInformation().center, rover->getRealLocationInformation().orientation);
    this->update();
}

void GridScene::toggleError() {
    middleFrontSensor->simulateWithError = !middleFrontSensor->simulateWithError;
    leftFrontSensor->simulateWithError = !leftFrontSensor->simulateWithError;
    rightFrontSensor->simulateWithError = !rightFrontSensor->simulateWithError;
    farLeftSensor->simulateWithError = !farLeftSensor->simulateWithError;
    farRightSensor->simulateWithError = !farRightSensor->simulateWithError;
//    rightSideSensor->simulateWithError = !rightSideSensor->simulateWithError;
//    leftSideSensor->simulateWithError = !leftSideSensor->simulateWithError;
}

QPointF GridScene::rotatePoint(float originX, float originY, float pointX, float pointY, double rotationAngle) {
    int translatedX = pointX - originX;
    int translatedY = pointY - originY;
    rotationAngle = rotationAngle*M_PI/180.0;
    QPointF point = QPointF(translatedX * cos(rotationAngle) - translatedY * sin(rotationAngle) + originX,
                  translatedX * sin(rotationAngle) + translatedY * cos(rotationAngle) + originY);
    return point;
}

