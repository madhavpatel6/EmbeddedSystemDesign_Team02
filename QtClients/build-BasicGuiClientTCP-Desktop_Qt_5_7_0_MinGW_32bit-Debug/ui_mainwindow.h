/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *commStatButton;
    QTextEdit *targetLocatorTextEdit;
    QLabel *targetLocatorLabel;
    QLabel *searcherMoverLabel;
    QLabel *pathFindingLabel;
    QLabel *targetGrabberLabel;
    QTextEdit *searcherMoverTextEdit;
    QTextEdit *pathFinderTextEdit;
    QTextEdit *targetGrabberTextEdit;
    QTextEdit *sentCommStatTextEdit;
    QLabel *sentCommStatLabel;
    QCheckBox *connectToServerCheckBox;
    QLabel *connectedToServerLabel;
    QTextEdit *connectedToServerTextEdit;
    QMenuBar *menuBar;
    QMenu *menuBasicGuiClientTCP;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(497, 349);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        commStatButton = new QPushButton(centralWidget);
        commStatButton->setObjectName(QStringLiteral("commStatButton"));
        commStatButton->setGeometry(QRect(60, 240, 101, 41));
        targetLocatorTextEdit = new QTextEdit(centralWidget);
        targetLocatorTextEdit->setObjectName(QStringLiteral("targetLocatorTextEdit"));
        targetLocatorTextEdit->setGeometry(QRect(110, 170, 371, 31));
        targetLocatorLabel = new QLabel(centralWidget);
        targetLocatorLabel->setObjectName(QStringLiteral("targetLocatorLabel"));
        targetLocatorLabel->setGeometry(QRect(10, 170, 91, 31));
        searcherMoverLabel = new QLabel(centralWidget);
        searcherMoverLabel->setObjectName(QStringLiteral("searcherMoverLabel"));
        searcherMoverLabel->setGeometry(QRect(10, 120, 91, 31));
        pathFindingLabel = new QLabel(centralWidget);
        pathFindingLabel->setObjectName(QStringLiteral("pathFindingLabel"));
        pathFindingLabel->setGeometry(QRect(10, 60, 71, 41));
        targetGrabberLabel = new QLabel(centralWidget);
        targetGrabberLabel->setObjectName(QStringLiteral("targetGrabberLabel"));
        targetGrabberLabel->setGeometry(QRect(10, 20, 81, 31));
        searcherMoverTextEdit = new QTextEdit(centralWidget);
        searcherMoverTextEdit->setObjectName(QStringLiteral("searcherMoverTextEdit"));
        searcherMoverTextEdit->setGeometry(QRect(110, 120, 371, 31));
        pathFinderTextEdit = new QTextEdit(centralWidget);
        pathFinderTextEdit->setObjectName(QStringLiteral("pathFinderTextEdit"));
        pathFinderTextEdit->setGeometry(QRect(110, 70, 371, 31));
        targetGrabberTextEdit = new QTextEdit(centralWidget);
        targetGrabberTextEdit->setObjectName(QStringLiteral("targetGrabberTextEdit"));
        targetGrabberTextEdit->setGeometry(QRect(110, 20, 371, 31));
        sentCommStatTextEdit = new QTextEdit(centralWidget);
        sentCommStatTextEdit->setObjectName(QStringLiteral("sentCommStatTextEdit"));
        sentCommStatTextEdit->setGeometry(QRect(330, 220, 41, 31));
        sentCommStatLabel = new QLabel(centralWidget);
        sentCommStatLabel->setObjectName(QStringLiteral("sentCommStatLabel"));
        sentCommStatLabel->setGeometry(QRect(240, 220, 101, 31));
        connectToServerCheckBox = new QCheckBox(centralWidget);
        connectToServerCheckBox->setObjectName(QStringLiteral("connectToServerCheckBox"));
        connectToServerCheckBox->setGeometry(QRect(60, 210, 111, 17));
        connectedToServerLabel = new QLabel(centralWidget);
        connectedToServerLabel->setObjectName(QStringLiteral("connectedToServerLabel"));
        connectedToServerLabel->setGeometry(QRect(220, 260, 101, 20));
        connectedToServerTextEdit = new QTextEdit(centralWidget);
        connectedToServerTextEdit->setObjectName(QStringLiteral("connectedToServerTextEdit"));
        connectedToServerTextEdit->setGeometry(QRect(330, 260, 41, 31));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 497, 21));
        menuBasicGuiClientTCP = new QMenu(menuBar);
        menuBasicGuiClientTCP->setObjectName(QStringLiteral("menuBasicGuiClientTCP"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuBasicGuiClientTCP->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        commStatButton->setText(QApplication::translate("MainWindow", "commStatButton", 0));
        targetLocatorLabel->setText(QApplication::translate("MainWindow", "Target Locator:", 0));
        searcherMoverLabel->setText(QApplication::translate("MainWindow", "Searcher Mover:", 0));
        pathFindingLabel->setText(QApplication::translate("MainWindow", "PathFinder:", 0));
        targetGrabberLabel->setText(QApplication::translate("MainWindow", "Target Grabber", 0));
        sentCommStatLabel->setText(QApplication::translate("MainWindow", "Sent commStat:", 0));
        connectToServerCheckBox->setText(QApplication::translate("MainWindow", "Connect to Server", 0));
        connectedToServerLabel->setText(QApplication::translate("MainWindow", "Connect to Server:", 0));
        menuBasicGuiClientTCP->setTitle(QApplication::translate("MainWindow", "BasicGuiClientTCP", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
