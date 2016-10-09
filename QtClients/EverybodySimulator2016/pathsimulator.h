#ifndef PATHSIMULATOR_H
#define PATHSIMULATOR_H

#include <QWidget>
#include <initialization.h>
#include <QGraphicsScene>

namespace Ui {
class PathSimulator;
}

class PathSimulator : public QWidget
{
    Q_OBJECT

public:
    explicit PathSimulator(QWidget *parent = 0);
    ~PathSimulator();
    void setConfig(initialization* config);

private:
    Ui::PathSimulator *ui;
    initialization* config;

    QGraphicsScene scene;
};

#endif // PATHSIMULATOR_H
