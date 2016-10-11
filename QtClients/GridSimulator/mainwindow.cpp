#include "mainwindow.h"
#include <QGraphicsView>
#include <QThread>
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    connect(clearOccupanyGridButton, SIGNAL(released()), this, SLOT(handleGridClear()));
    connect(simulateButton, SIGNAL(released()), this, SLOT(handleSimulate()));
    connect(grid, SIGNAL(updateCursorPosition(int,int)), this, SLOT(updateCursorPosition(int,int)));
    sensorReadingTimer = new QTimer();
    sensorReadingTimer->setInterval(50);
    sensorReadingTimer->start();
    connect(sensorReadingTimer, SIGNAL(timeout()), this, SLOT(handleSimulate()));
    connect(showObjects, SIGNAL(released()), this, SLOT(handleShowObjects()));
    connect(grid, SIGNAL(updateRoverPosition(float,float,float)), this, SLOT(handleRoverCoordinateUpdate(float,float,float)));
//    grid->UpdateSensorReading(GridScene::MIDDLESENSOR, 13*GridScene::CELL_SIZE);
//    grid->addLine(142,144,321,124);
//    grid->addLine(12,38,22,88);
    middleFIRDistance->setText(QString::number(20));
    rightFIRDistance->setText(QString::number(20));
    leftFIRDistance->setText(QString::number(20));
}

MainWindow::~MainWindow()
{
//    delete grid;
//    delete verticalLayout1;
//    delete verticalLayout2;
//    delete horizontalLayout;
}

void MainWindow::setupUi(QWidget* mainwindow) {
    if (mainwindow->objectName().isEmpty())
        mainwindow->setObjectName(QString("Occupany Grid Simulator"));
    horizontalLayout = new QHBoxLayout(mainwindow);
    horizontalLayout->setSpacing(5);
    horizontalLayout->setContentsMargins(5,5,5,5);
    horizontalLayout->setObjectName(QString("horizontalLayout"));
    grid = new GridScene();
    horizontalLayout->addWidget(grid);
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
    verticalLayout1->addWidget(roverLocXLabel);
    verticalLayout1->addWidget(xRoverLoc);
    verticalLayout1->addWidget(roverLocYLabel);
    verticalLayout1->addWidget(yRoverLoc);
    verticalLayout1->addWidget(roverAngleLabel);
    verticalLayout1->addWidget(roverAngle);
    verticalLayout1->addWidget(clearOccupanyGridButton);
    verticalLayout1->addWidget(cursorPosition);
    verticalLayout1->addItem(verticalSpacer1);
    middleFIRDistance = new QLineEdit();
    rightFIRDistance = new QLineEdit();
    leftFIRDistance = new QLineEdit();
    showObjects = new QPushButton("Hide Objects");
    verticalLayout2->addWidget(leftFIRDistanceLabel);
    verticalLayout2->addWidget(leftFIRDistance);
    verticalLayout2->addWidget(middleFIRDistanceLabel);
    verticalLayout2->addWidget(middleFIRDistance);
    verticalLayout2->addWidget(rightFIRDistanceLabel);
    verticalLayout2->addWidget(rightFIRDistance);
    verticalLayout2->addWidget(simulateButton);
    verticalLayout2->addWidget(showObjects);
    verticalLayout2->addItem(verticalSpacer2);
    setFocusProxy(grid);
}

void MainWindow::handleGridClear() {
    grid->reset();
}

void MainWindow::handleRoverCoordinateUpdate(float x, float y, float angle) {
    xRoverLoc->setText(QString::number(x));
    yRoverLoc->setText(QString::number(y));
    roverAngle->setText(QString::number(angle));
}

void MainWindow::handleSimulate() {
    grid->setFocus();
    grid->updateSensorReading();
}

void MainWindow::handleShowObjects() {
    if(grid->showObjects) {
        grid->showObjects = false;
        showObjects->setText("Show Objects");
    }
    else if(!grid->showObjects) {
        grid->showObjects = true;
        showObjects->setText("Hide Objects");
    }
}

void MainWindow::updateCursorPosition(int x, int y) {
    cursorPosition->setText(QString("%1\t%2").arg(x, 2, 10, QChar('0')).arg(y, 2, 10, QChar('0')));
}
