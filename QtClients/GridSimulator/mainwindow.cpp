#include "mainwindow.h"
#include <QGraphicsView>
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    grid->addLine(142,144,321,124);
//    grid->addLine(12,38,22,88);
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
//    button = new QPushButton();
    verticalLayout->addWidget(grid);
    verticalLayout->setAlignment(Qt::AlignCenter);
}

