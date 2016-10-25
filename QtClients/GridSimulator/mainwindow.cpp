#include "mainwindow.h"
#include <QGraphicsView>
#include <QThread>
#include <time.h>
#include <QKeyEvent>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
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
    movementTimer->setInterval(333); //original 333
    sensorReadingTimer->setInterval(50); //original 50
    sensorReadingTimer->start();
    connect(sensorReadingTimer, SIGNAL(timeout()), this, SLOT(handleSimulate()));
    connect(showObjects, SIGNAL(released()), this, SLOT(handleShowObjects()));
    connect(gridwidget, SIGNAL(updateRoverPosition(float,float,float)), this, SLOT(handleRoverCoordinateUpdate(float,float,float)));
    connect(errorButton, SIGNAL(released()), this, SLOT(toggleError()));
    connect(simulateMap, SIGNAL(released()), this, SLOT(handleSimulateMap()));
    connect(movementTimer, SIGNAL(timeout()), this, SLOT(handleRoverMovementSimulation()));
    connect(loadSimulationButton, SIGNAL(released()), this, SLOT(handleLoadSimulation()));
    connect(saveSimulationButton, SIGNAL(released()), this, SLOT(handleSaveSimulation()));
    connect(resetButton, SIGNAL(released()), this, SLOT(handleSimulationReset()));

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
    resetButton = new QPushButton("Reset Simulator");
    comboBox = new QComboBox();
    comboBox->addItem(QString("Map 1"));
    SimulationSetupType map1;
    simulateMap = new QPushButton("Start Simulation");
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
    verticalLayout1->addWidget(resetButton);
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

void MainWindow::handleSimulationReset() {
    gridwidget->resetGrid();
    gridwidget->keys.clear();
    sensorReadingTimer->stop();
    movementTimer->stop();
    gridwidget->resetRoverPosition();
    gridwidget->resetGrid();
    gridwidget->objects.clear();
    sensorReadingTimer->start();
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
    QString filename = QFileDialog::getOpenFileName(this, tr("Load Simulation"),
                                "",
                                tr("Simulation (*.txt)"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QTextStream in(&file);
    typedef enum statetype { Polygons, Keys } statetype;
    statetype state = Polygons;
    while(!in.atEnd()) {
        QString line = in.readLine();
        switch(state) {
            case Polygons: {
                if(line == "END") {
                    state = Keys;
                    break;
                }
                QPolygonF poly;
                for(int i = 0; i < 8; i+=2) {
                    poly << QPointF(QString(line.split(" ").at(i)).toFloat(), QString(line.split(" ").at(i + 1)).toFloat());
                }
                qDebug() << poly;
                gridwidget->objects.append(poly);
                break;
            }
            case Keys: {
                if(line == "END") {
                    break;
                }
                for(int i = 0; i < line.split(" ").size(); i++) {
                    gridwidget->keys.append(QString(line.split(" ").at(i)).toInt());
                }
                break;
            }
        }
    }
    file.close();
    sensorReadingTimer->stop();
    gridwidget->resetRoverPosition();
    gridwidget->resetGrid();
    gridwidget->update();
    sensorReadingTimer->start();
}

void MainWindow::handleSaveSimulation() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Simulation"),
                                "",
                                tr("Simulation (*.txt)"));
    QFile fileout(filename);
    if (fileout.open(QFile::ReadWrite)){
        QTextStream out(&fileout);
        for (size_t itr = 0; itr < gridwidget->objects.size(); itr++){
         for(int i = 0; i < gridwidget->objects[itr].size(); i++) {
             out << gridwidget->objects[itr][i].x() << " " << gridwidget->objects[itr][i].y() << " ";
         }
         out << "\n";
        }
        out << "END\n";
        for(size_t itr = 0; itr < gridwidget->keys.size(); itr++) {
            out << gridwidget->keys[itr] << " ";
        }
        out << "END\n";
        fileout.close();
    }
}

void MainWindow::handleSimulateMap() {
    if(simulateMap->text() == "Start Simulation") {
        index = 0;
        sensorReadingTimer->stop();
        movementTimer->start();
        simulateMap->setText("Pause Simulation");
        sensorReadingTimer->start();
    }
    else if(simulateMap->text() == "Pause Simulation"){
        sensorReadingTimer->stop();
        movementTimer->stop();
        simulateMap->setText("Start Simulation");
    }
}

void MainWindow::handleRoverMovementSimulation() {
    if(index < gridwidget->keys.size()) {
        gridwidget->recordKeys = false;
        QKeyEvent event = QKeyEvent (QEvent::KeyPress, gridwidget->keys[index], Qt::NoModifier);
        QApplication::sendEvent(gridwidget, &event);
        index++;
    }
    else {
        gridwidget->recordKeys = true;
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
