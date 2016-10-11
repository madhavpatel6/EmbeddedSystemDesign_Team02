#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include "gridscene.h"
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GridScene *grid;
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
    QPushButton* showObjects;
public slots:
    void handleGridClear();
    void handleSimulate();
    void handleShowObjects();
    void updateCursorPosition(int x, int y);
    void handleRoverCoordinateUpdate(float x, float y, float angle);
private:
    void setupUi(QWidget* mainwindow);
};

#endif // MAINWINDOW_H
