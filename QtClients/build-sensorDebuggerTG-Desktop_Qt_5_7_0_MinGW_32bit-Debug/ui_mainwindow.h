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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPlainTextEdit *IR_0PlainTextEdit;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QPlainTextEdit *IR_1PlainTextEdit;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QPlainTextEdit *IR_2PlainTextEdit;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_5;
    QPushButton *startSensingButton;
    QLabel *numberOfSensorValuesReceivedLabel;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QLabel *IR_0Label;
    QLabel *IR_0CountLabel;
    QHBoxLayout *horizontalLayout_3;
    QLabel *IR_1Label;
    QLabel *IR_1CountLabel;
    QHBoxLayout *horizontalLayout_2;
    QLabel *IR_2Label;
    QLabel *IR_2CountLabel;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_8;
    QLabel *ipAddressLabel;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *serverIPLineEdit;
    QHBoxLayout *horizontalLayout_9;
    QLabel *portLabel;
    QSpacerItem *horizontalSpacer;
    QLineEdit *portLineEdit;
    QHBoxLayout *horizontalLayout_6;
    QLabel *connectedLabel;
    QLabel *connectionStatusLabel;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *connectToServerButton;
    QPushButton *disconnectFromServerButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(634, 382);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 134, 631, 191));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        IR_0PlainTextEdit = new QPlainTextEdit(layoutWidget);
        IR_0PlainTextEdit->setObjectName(QStringLiteral("IR_0PlainTextEdit"));

        verticalLayout->addWidget(IR_0PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        IR_1PlainTextEdit = new QPlainTextEdit(layoutWidget);
        IR_1PlainTextEdit->setObjectName(QStringLiteral("IR_1PlainTextEdit"));

        verticalLayout_2->addWidget(IR_1PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_3);

        IR_2PlainTextEdit = new QPlainTextEdit(layoutWidget);
        IR_2PlainTextEdit->setObjectName(QStringLiteral("IR_2PlainTextEdit"));

        verticalLayout_3->addWidget(IR_2PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout_3);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(250, 0, 381, 131));
        verticalLayout_5 = new QVBoxLayout(layoutWidget1);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        startSensingButton = new QPushButton(layoutWidget1);
        startSensingButton->setObjectName(QStringLiteral("startSensingButton"));

        verticalLayout_5->addWidget(startSensingButton);

        numberOfSensorValuesReceivedLabel = new QLabel(layoutWidget1);
        numberOfSensorValuesReceivedLabel->setObjectName(QStringLiteral("numberOfSensorValuesReceivedLabel"));
        numberOfSensorValuesReceivedLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(numberOfSensorValuesReceivedLabel);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        IR_0Label = new QLabel(layoutWidget1);
        IR_0Label->setObjectName(QStringLiteral("IR_0Label"));

        horizontalLayout_4->addWidget(IR_0Label);

        IR_0CountLabel = new QLabel(layoutWidget1);
        IR_0CountLabel->setObjectName(QStringLiteral("IR_0CountLabel"));

        horizontalLayout_4->addWidget(IR_0CountLabel);


        horizontalLayout_5->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        IR_1Label = new QLabel(layoutWidget1);
        IR_1Label->setObjectName(QStringLiteral("IR_1Label"));

        horizontalLayout_3->addWidget(IR_1Label);

        IR_1CountLabel = new QLabel(layoutWidget1);
        IR_1CountLabel->setObjectName(QStringLiteral("IR_1CountLabel"));

        horizontalLayout_3->addWidget(IR_1CountLabel);


        horizontalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        IR_2Label = new QLabel(layoutWidget1);
        IR_2Label->setObjectName(QStringLiteral("IR_2Label"));

        horizontalLayout_2->addWidget(IR_2Label);

        IR_2CountLabel = new QLabel(layoutWidget1);
        IR_2CountLabel->setObjectName(QStringLiteral("IR_2CountLabel"));

        horizontalLayout_2->addWidget(IR_2CountLabel);


        horizontalLayout_5->addLayout(horizontalLayout_2);


        verticalLayout_5->addLayout(horizontalLayout_5);

        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(-1, 1, 251, 131));
        verticalLayout_6 = new QVBoxLayout(layoutWidget2);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        ipAddressLabel = new QLabel(layoutWidget2);
        ipAddressLabel->setObjectName(QStringLiteral("ipAddressLabel"));

        horizontalLayout_8->addWidget(ipAddressLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);

        serverIPLineEdit = new QLineEdit(layoutWidget2);
        serverIPLineEdit->setObjectName(QStringLiteral("serverIPLineEdit"));

        horizontalLayout_8->addWidget(serverIPLineEdit);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        portLabel = new QLabel(layoutWidget2);
        portLabel->setObjectName(QStringLiteral("portLabel"));

        horizontalLayout_9->addWidget(portLabel);

        horizontalSpacer = new QSpacerItem(71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer);

        portLineEdit = new QLineEdit(layoutWidget2);
        portLineEdit->setObjectName(QStringLiteral("portLineEdit"));

        horizontalLayout_9->addWidget(portLineEdit);


        verticalLayout_4->addLayout(horizontalLayout_9);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        connectedLabel = new QLabel(layoutWidget2);
        connectedLabel->setObjectName(QStringLiteral("connectedLabel"));

        horizontalLayout_6->addWidget(connectedLabel);

        connectionStatusLabel = new QLabel(layoutWidget2);
        connectionStatusLabel->setObjectName(QStringLiteral("connectionStatusLabel"));

        horizontalLayout_6->addWidget(connectionStatusLabel);


        verticalLayout_4->addLayout(horizontalLayout_6);


        verticalLayout_6->addLayout(verticalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        connectToServerButton = new QPushButton(layoutWidget2);
        connectToServerButton->setObjectName(QStringLiteral("connectToServerButton"));

        horizontalLayout_7->addWidget(connectToServerButton);

        disconnectFromServerButton = new QPushButton(layoutWidget2);
        disconnectFromServerButton->setObjectName(QStringLiteral("disconnectFromServerButton"));

        horizontalLayout_7->addWidget(disconnectFromServerButton);


        verticalLayout_6->addLayout(horizontalLayout_7);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 634, 17));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label->setText(QApplication::translate("MainWindow", "IR_0", 0));
        label_2->setText(QApplication::translate("MainWindow", "IR_1", 0));
        label_3->setText(QApplication::translate("MainWindow", "IR_2", 0));
        startSensingButton->setText(QApplication::translate("MainWindow", "Start Reading Sensors", 0));
        numberOfSensorValuesReceivedLabel->setText(QApplication::translate("MainWindow", "Number of Sensor Values Received", 0));
        IR_0Label->setText(QApplication::translate("MainWindow", "IR_0:", 0));
        IR_0CountLabel->setText(QApplication::translate("MainWindow", "0", 0));
        IR_1Label->setText(QApplication::translate("MainWindow", "IR_1:", 0));
        IR_1CountLabel->setText(QApplication::translate("MainWindow", "0", 0));
        IR_2Label->setText(QApplication::translate("MainWindow", "IR_2:", 0));
        IR_2CountLabel->setText(QApplication::translate("MainWindow", "0", 0));
        ipAddressLabel->setText(QApplication::translate("MainWindow", "IP Address:", 0));
        serverIPLineEdit->setText(QApplication::translate("MainWindow", "192.168.0.197", 0));
        portLabel->setText(QApplication::translate("MainWindow", "Port: ", 0));
        portLineEdit->setText(QApplication::translate("MainWindow", "2000", 0));
        connectedLabel->setText(QApplication::translate("MainWindow", "Connected:", 0));
        connectionStatusLabel->setText(QApplication::translate("MainWindow", "No", 0));
        connectToServerButton->setText(QApplication::translate("MainWindow", "Connect To Server", 0));
        disconnectFromServerButton->setText(QApplication::translate("MainWindow", "Disconnect From Server", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
