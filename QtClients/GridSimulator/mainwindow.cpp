#include "mainwindow.h"
#include <QGraphicsView>
#include <QThread>
#include <time.h>
#include <QKeyEvent>
#include <QApplication>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    srand(time(NULL));
    setupUi(this);
    connect(clearOccupanyGridButton, SIGNAL(released()), this, SLOT(handleGridClear()));
    connect(simulateButton, SIGNAL(released()), this, SLOT(handleSimulate()));
    connect(gridwidget, SIGNAL(updateCursorPosition(int,int)), this, SLOT(updateCursorPosition(int,int)));
    sensorReadingTimer = new QTimer();
    movementTimer = new QTimer();
    movementTimer->setInterval(100);
    sensorReadingTimer->setInterval(50);
    sensorReadingTimer->start();
    connect(sensorReadingTimer, SIGNAL(timeout()), this, SLOT(handleSimulate()));
    connect(showObjects, SIGNAL(released()), this, SLOT(handleShowObjects()));
    connect(gridwidget, SIGNAL(updateRoverPosition(float,float,float)), this, SLOT(handleRoverCoordinateUpdate(float,float,float)));
    connect(errorButton, SIGNAL(released()), this, SLOT(toggleError()));
    connect(simulateMap, SIGNAL(released()), this, SLOT(handleSimulateMap()));
    connect(movementTimer, SIGNAL(timeout()), this, SLOT(handleRoverMovementSimulation()));
    connect(loadSimulationButton, SIGNAL(released()), this, SLOT(handleLoadSimulation()));
    connect(saveSimulationButton, SIGNAL(released()), this, SLOT(handleSaveSimulation()));
//    grid->UpdateSensorReading(GridScene::MIDDLESENSOR, 13*GridScene::CELL_SIZE);
//    grid->addLine(142,144,321,124);
//    grid->addLine(12,38,22,88);
    middleFIRDistance->setText(QString::number(20));
    rightFIRDistance->setText(QString::number(20));
    leftFIRDistance->setText(QString::number(20));
}

MainWindow::~MainWindow()
{
    delete gridwidget;
    delete verticalLayout1;
    delete verticalLayout2;
    delete horizontalLayout;
}

void MainWindow::setupUi(QWidget* mainwindow) {
    if (mainwindow->objectName().isEmpty())
        mainwindow->setObjectName(QString("Occupany Grid Simulator"));
    horizontalLayout = new QHBoxLayout(mainwindow);
    horizontalLayout->setSpacing(5);
    horizontalLayout->setContentsMargins(5,5,5,5);
    horizontalLayout->setObjectName(QString("horizontalLayout"));
    gridwidget = new GridScene();
    horizontalLayout->addWidget(gridwidget);
    horizontalLayout->setAlignment(Qt::AlignCenter);

    verticalLayout1 = new QVBoxLayout();
    horizontalLayout->addLayout(verticalLayout1);
    clearOccupanyGridButton = new QPushButton();
    simulateButton = new QPushButton();
    verticalLayout2 = new QVBoxLayout();

    horizontalLayout->addLayout(verticalLayout2);

    simulateButton->setText("Update Map");
    clearOccupanyGridButton->setText("Clear Occupany Grid");

    xRoverLoc = new QLineEdit();
    yRoverLoc = new QLineEdit();
    roverAngle = new QLineEdit();
    roverLocXLabel = new QLabel("X Location");
    roverLocYLabel = new QLabel("Y Location");
    roverAngleLabel = new QLabel("Angle");
    middleFIRDistanceLabel = new QLabel("Middle Front IR Sensor (1 cm = 1 grid cell)");
    rightFIRDistanceLabel = new QLabel("Right Front IR Sensor (1 cm = 1 grid cell)");
    leftFIRDistanceLabel = new QLabel("Left Front IR Sensor (1 cm = 1 grid cell)");
    cursorPosition = new QLabel("");
    verticalSpacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalSpacer2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    showObjects = new QPushButton("Hide Objects");
    errorButton = new QPushButton("Enable Errors");
    loadSimulationButton = new QPushButton("Load Simulation");
    saveSimulationButton = new QPushButton("Save Simulation");
    comboBox = new QComboBox();
    comboBox->addItem(QString("Map 1"));
    SimulationSetupType map1;
    map1.objects = {
        QPolygonF() << QPointF(204.593,735.672) << QPointF(205.41,646.676) << QPointF(276.407,647.328) << QPointF(275.59,736.324),
        QPolygonF() << QPointF(577.053,687.591) << QPointF(626.023,580.233) << QPointF(757.947,640.409) << QPointF(708.977,747.767),
        QPolygonF() << QPointF(640.548,447.495) << QPointF(641.463,404.505) << QPointF(688.452,405.505) << QPointF(687.537,448.495),
        QPolygonF() << QPointF(417.426,619.445) << QPointF(405.073,583.509) << QPointF(448.574,568.555) << QPointF(460.927,604.491),
        QPolygonF() << QPointF(73.7421,445.198) << QPointF(79.4961,410.675) << QPointF(314.258,449.802) << QPointF(308.504,484.325),
        QPolygonF() << QPointF(469.129,368.441) << QPointF(437.679,328.292) << QPointF(462.871,308.559) << QPointF(494.321,348.708),
        QPolygonF() << QPointF(181.608,234.682) << QPointF(116.712,178.251) << QPointF(178.392,107.318) << QPointF(243.288,163.749),
        QPolygonF() << QPointF(617.535,104.487) << QPointF(578.775,137.633)  << QPointF(668.465,242.513) << QPointF(707.225,209.367)
    };
    map1.keys = {
        Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,
        Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,
        Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,
        Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,
        Qt::Key_W ,Qt::Key_W ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,
        Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,
        Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,
        Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,
        Qt::Key_A ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,
        Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,
        Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,
        Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,
        Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,
        Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,
        Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_W ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_A ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_D ,Qt::Key_D ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_W ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A ,Qt::Key_A
    };
    polygons.push_back(map1);

    simulateMap = new QPushButton("Simulate Map");
    verticalLayout1->addWidget(roverLocXLabel);
    verticalLayout1->addWidget(xRoverLoc);
    verticalLayout1->addWidget(roverLocYLabel);
    verticalLayout1->addWidget(yRoverLoc);
    verticalLayout1->addWidget(roverAngleLabel);
    verticalLayout1->addWidget(roverAngle);
    verticalLayout1->addWidget(clearOccupanyGridButton);
    verticalLayout1->addWidget(simulateButton);
    verticalLayout1->addWidget(showObjects);
    verticalLayout1->addWidget(errorButton);
    verticalLayout1->addWidget(comboBox);
    verticalLayout1->addWidget(loadSimulationButton);
    verticalLayout1->addWidget(saveSimulationButton);
    verticalLayout1->addWidget(simulateMap);
    verticalLayout1->addWidget(cursorPosition);
    verticalLayout1->addItem(verticalSpacer1);
    middleFIRDistance = new QLineEdit();
    rightFIRDistance = new QLineEdit();
    leftFIRDistance = new QLineEdit();
    verticalLayout2->addItem(verticalSpacer2);
    setFocusProxy(gridwidget);
}

void MainWindow::handleGridClear() {
    gridwidget->resetGrid();
}

void MainWindow::handleRoverCoordinateUpdate(float x, float y, float angle) {
    xRoverLoc->setText(QString::number(x));
    yRoverLoc->setText(QString::number(y));
    roverAngle->setText(QString::number(angle));
}

void MainWindow::handleSimulate() {
    gridwidget->setFocus();
    gridwidget->updateSensorReading();
}

void MainWindow::handleShowObjects() {
    if(gridwidget->showObjects) {
        gridwidget->showObjects = false;
        showObjects->setText("Show Objects");
    }
    else if(!gridwidget->showObjects) {
        gridwidget->showObjects = true;
        showObjects->setText("Hide Objects");
    }
}

void MainWindow::handleLoadSimulation() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Simulation"),
                                "",
                                tr("Simulation (*.sim)"));

}

void MainWindow::handleSaveSimulation() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Simulation"),
                                "",
                                tr("Simulation (*.txt)"));
    QFile fileout(filename);
    if (fileout.open(QFile::ReadWrite)){
     QTextStream out(&fileout);
     for (size_t itr = 0; itr < gridwidget->rects.size(); itr++){
         for(int i = 0; i < gridwidget->rects[itr].size(); i++) {
             out << gridwidget->rects[itr][i].x() << " " << gridwidget->rects[itr][i].y() << " ";
         }
         out << "\n";
     }
     out << "END";
     fileout.close();
    }
}

void MainWindow::handleSimulateMap() {
    if(simulateMap->text() == "Simulate Map") {
        gridwidget->rects = polygons[comboBox->currentIndex()].objects;
        index = 0;
        sensorReadingTimer->stop();
        gridwidget->resetRoverPosition();
        gridwidget->resetGrid();
        movementTimer->start();
        simulateMap->setText("Free Roam");
        sensorReadingTimer->start();
    }
    else if(simulateMap->text() == "Free Roam"){
        sensorReadingTimer->stop();
        movementTimer->stop();
        gridwidget->resetRoverPosition();
        gridwidget->resetGrid();
        gridwidget->rects.clear();
        simulateMap->setText("Simulate Map");
        sensorReadingTimer->start();
    }
}

void MainWindow::handleRoverMovementSimulation() {
    if(index < polygons[comboBox->currentIndex()].keys.size()) {
        QKeyEvent event = QKeyEvent (QEvent::KeyPress, polygons[comboBox->currentIndex()].keys[index], Qt::NoModifier);
        QApplication::sendEvent(gridwidget, &event);
        index++;
    }
    else {
        movementTimer->stop();
    }
}

void MainWindow::updateCursorPosition(int x, int y) {
    cursorPosition->setText(QString("%1\t%2").arg(x, 2, 10, QChar('0')).arg(y, 2, 10, QChar('0')));
}

void MainWindow::toggleError() {
    gridwidget->toggleError();
    if(errorButton->text() == "Enable Errors") {
        errorButton->setText("Disable Errors");
    }
    else {
        errorButton->setText("Enable Errors");
    }
    gridwidget->resetGrid();
}
