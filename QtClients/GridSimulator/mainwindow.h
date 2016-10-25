#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QComboBox>
#include <QVector>
#include "gridscene.h"
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GridScene *gridwidget;
    QHBoxLayout* horizontalLayout;
    QVBoxLayout* verticalLayout1;
    QVBoxLayout* verticalLayout2;
    QPushButton* clearOccupanyGridButton;
    QPushButton* simulateButton;
    QLineEdit* xRoverLoc;
    QLineEdit* yRoverLoc;
    QLineEdit* roverAngle;
    QLineEdit* middleFIRDistance;
    QLineEdit* rightFIRDistance;
    QLineEdit* leftFIRDistance;
    QLabel* roverLocXLabel;
    QLabel* roverLocYLabel;
    QLabel* roverAngleLabel;
    QLabel* middleFIRDistanceLabel;
    QLabel* rightFIRDistanceLabel;
    QLabel* leftFIRDistanceLabel;
    QSpacerItem *verticalSpacer1;
    QSpacerItem *verticalSpacer2;
    QLabel* cursorPosition;
    QTimer* sensorReadingTimer;
    QTimer* movementTimer;
    QPushButton* showObjects;
    QPushButton* resetButton;
    QPushButton* errorButton;
    QPushButton* loadSimulationButton;
    QPushButton* saveSimulationButton;
    QPushButton* simulateMap;
    QComboBox* comboBox;
    typedef struct SimulationSetupType {
        QVector<QPolygonF> objects;
        QVector<int> keys;
    } SimulationSetupType;

    QVector<SimulationSetupType> polygons;
    QVector<int> keys;
    int index;
public slots:
    void handleSimulationReset();
    void handleGridClear();
    void handleSimulate();
    void toggleError();
    void handleShowObjects();
    void handleSimulateMap();
    void handleRoverMovementSimulation();
    void updateCursorPosition(int x, int y);
    void handleRoverCoordinateUpdate(float x, float y, float angle);
    void handleSaveSimulation();
    void handleLoadSimulation();
private:
    void setupUi(QWidget* mainwindow);
};

#endif // MAINWINDOW_H
