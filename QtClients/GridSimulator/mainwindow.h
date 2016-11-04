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
#include <QRadioButton>

#include "gridscene.h"
#include "clientsocket.h"
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ClientSocket* socket;
    GridScene *gridwidget;
    QHBoxLayout* horizontalLayout;
    QVBoxLayout* verticalLayout1;
    QVBoxLayout* verticalLayout2;
    QPushButton* clearOccupanyGridButton;
    QPushButton* simulateButton;
    QLineEdit* ipaddress;
    QPushButton* connectToServer;
    QLineEdit* xRoverLoc;
    QLineEdit* yRoverLoc;
    QLineEdit* roverAngle;
    QLineEdit* middleFIRDistance;
    QLineEdit* rightFIRDistance;
    QLineEdit* leftFIRDistance;
    QRadioButton* simulationMode;
    QRadioButton* requestMode;
    QPushButton* requestOccupanyGridButton;
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
    void handleRequestOccupanyGrid() {
        if(requestMode->isChecked()) {
            socket->sendRequest();
        }
    }

    void handleConnect() {
        socket->connectToHost(ipaddress->text(), 2000);
    }

    void handleSimulateRadioButtion(bool toggled) {
        if(simulationMode->isChecked()) {
            handleSimulationReset();
            sensorReadingTimer->start();
            gridwidget->setFocus();
        }
        else{
            sensorReadingTimer->stop();
            handleSimulationReset();
        }
    }

    void HostConnectionEvent(bool connected) {
        qDebug() << "Host Connection Event " << connected;
        if(connected) {
            connectToServer->setText(QString("Connected To Server"));
        }
        else {
            connectToServer->setText(QString("Connect To Server"));
        }
    }

private:
    void setupUi(QWidget* mainwindow);
};

#endif // MAINWINDOW_H
