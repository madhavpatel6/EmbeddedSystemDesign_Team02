#include "pathsimulator.h"
#include "ui_pathsimulator.h"

PathSimulator::PathSimulator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PathSimulator)
{
    ui->setupUi(this);

    QRectF roverR(0,0,40,80);
    QGraphicsRectItem *rover = scene.addRect(roverR);


    ui->graphicsView->setScene(&scene);

    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->graphicsView->show();
    ui->graphicsView->ensureVisible((QGraphicsItem*)rover);
}

PathSimulator::~PathSimulator()
{
    delete ui;
}

void PathSimulator::setConfig(initialization* config){
    this->config = config;
}
