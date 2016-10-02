#include "mainwindow.h"
#include <QGraphicsView>
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
}

MainWindow::~MainWindow()
{
    delete grid;
    delete verticalLayout;
}

void MainWindow::setupUi(QWidget* mainwindow) {
    if (mainwindow->objectName().isEmpty())
        mainwindow->setObjectName(QString("Occupany Grid Simulator"));
    verticalLayout = new QVBoxLayout(mainwindow);
    verticalLayout->setSpacing(5);
    verticalLayout->setContentsMargins(5,5,5,5);
    verticalLayout->setObjectName(QString("verticalLayout"));
    grid = new GridScene();
    verticalLayout->addWidget(grid);
}
