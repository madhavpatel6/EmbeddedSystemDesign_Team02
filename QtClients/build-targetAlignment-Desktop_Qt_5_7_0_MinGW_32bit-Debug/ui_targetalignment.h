/********************************************************************************
** Form generated from reading UI file 'targetalignment.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TARGETALIGNMENT_H
#define UI_TARGETALIGNMENT_H

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

class Ui_targetAlignment
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
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
    QWidget *layoutWidget_2;
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
    QWidget *layoutWidget_3;
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

    void setupUi(QMainWindow *targetAlignment)
    {
        if (targetAlignment->objectName().isEmpty())
            targetAlignment->setObjectName(QStringLiteral("targetAlignment"));
        targetAlignment->resize(659, 422);
        centralWidget = new QWidget(targetAlignment);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(350, 6, 291, 131));
        verticalLayout_5 = new QVBoxLayout(layoutWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        startSensingButton = new QPushButton(layoutWidget);
        startSensingButton->setObjectName(QStringLiteral("startSensingButton"));

        verticalLayout_5->addWidget(startSensingButton);

        numberOfSensorValuesReceivedLabel = new QLabel(layoutWidget);
        numberOfSensorValuesReceivedLabel->setObjectName(QStringLiteral("numberOfSensorValuesReceivedLabel"));
        numberOfSensorValuesReceivedLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(numberOfSensorValuesReceivedLabel);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        IR_0Label = new QLabel(layoutWidget);
        IR_0Label->setObjectName(QStringLiteral("IR_0Label"));

        horizontalLayout_4->addWidget(IR_0Label);

        IR_0CountLabel = new QLabel(layoutWidget);
        IR_0CountLabel->setObjectName(QStringLiteral("IR_0CountLabel"));

        horizontalLayout_4->addWidget(IR_0CountLabel);


        horizontalLayout_5->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        IR_1Label = new QLabel(layoutWidget);
        IR_1Label->setObjectName(QStringLiteral("IR_1Label"));

        horizontalLayout_3->addWidget(IR_1Label);

        IR_1CountLabel = new QLabel(layoutWidget);
        IR_1CountLabel->setObjectName(QStringLiteral("IR_1CountLabel"));

        horizontalLayout_3->addWidget(IR_1CountLabel);


        horizontalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        IR_2Label = new QLabel(layoutWidget);
        IR_2Label->setObjectName(QStringLiteral("IR_2Label"));

        horizontalLayout_2->addWidget(IR_2Label);

        IR_2CountLabel = new QLabel(layoutWidget);
        IR_2CountLabel->setObjectName(QStringLiteral("IR_2CountLabel"));

        horizontalLayout_2->addWidget(IR_2CountLabel);


        horizontalLayout_5->addLayout(horizontalLayout_2);


        verticalLayout_5->addLayout(horizontalLayout_5);

        layoutWidget_2 = new QWidget(centralWidget);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 140, 631, 191));
        horizontalLayout = new QHBoxLayout(layoutWidget_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(layoutWidget_2);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        IR_0PlainTextEdit = new QPlainTextEdit(layoutWidget_2);
        IR_0PlainTextEdit->setObjectName(QStringLiteral("IR_0PlainTextEdit"));

        verticalLayout->addWidget(IR_0PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(layoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        IR_1PlainTextEdit = new QPlainTextEdit(layoutWidget_2);
        IR_1PlainTextEdit->setObjectName(QStringLiteral("IR_1PlainTextEdit"));

        verticalLayout_2->addWidget(IR_1PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_3 = new QLabel(layoutWidget_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_3);

        IR_2PlainTextEdit = new QPlainTextEdit(layoutWidget_2);
        IR_2PlainTextEdit->setObjectName(QStringLiteral("IR_2PlainTextEdit"));

        verticalLayout_3->addWidget(IR_2PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout_3);

        layoutWidget_3 = new QWidget(centralWidget);
        layoutWidget_3->setObjectName(QStringLiteral("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(9, 7, 339, 134));
        verticalLayout_6 = new QVBoxLayout(layoutWidget_3);
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
        ipAddressLabel = new QLabel(layoutWidget_3);
        ipAddressLabel->setObjectName(QStringLiteral("ipAddressLabel"));

        horizontalLayout_8->addWidget(ipAddressLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);

        serverIPLineEdit = new QLineEdit(layoutWidget_3);
        serverIPLineEdit->setObjectName(QStringLiteral("serverIPLineEdit"));

        horizontalLayout_8->addWidget(serverIPLineEdit);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        portLabel = new QLabel(layoutWidget_3);
        portLabel->setObjectName(QStringLiteral("portLabel"));

        horizontalLayout_9->addWidget(portLabel);

        horizontalSpacer = new QSpacerItem(71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer);

        portLineEdit = new QLineEdit(layoutWidget_3);
        portLineEdit->setObjectName(QStringLiteral("portLineEdit"));

        horizontalLayout_9->addWidget(portLineEdit);


        verticalLayout_4->addLayout(horizontalLayout_9);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        connectedLabel = new QLabel(layoutWidget_3);
        connectedLabel->setObjectName(QStringLiteral("connectedLabel"));

        horizontalLayout_6->addWidget(connectedLabel);

        connectionStatusLabel = new QLabel(layoutWidget_3);
        connectionStatusLabel->setObjectName(QStringLiteral("connectionStatusLabel"));

        horizontalLayout_6->addWidget(connectionStatusLabel);


        verticalLayout_4->addLayout(horizontalLayout_6);


        verticalLayout_6->addLayout(verticalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        connectToServerButton = new QPushButton(layoutWidget_3);
        connectToServerButton->setObjectName(QStringLiteral("connectToServerButton"));

        horizontalLayout_7->addWidget(connectToServerButton);

        disconnectFromServerButton = new QPushButton(layoutWidget_3);
        disconnectFromServerButton->setObjectName(QStringLiteral("disconnectFromServerButton"));

        horizontalLayout_7->addWidget(disconnectFromServerButton);


        verticalLayout_6->addLayout(horizontalLayout_7);

        targetAlignment->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(targetAlignment);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 659, 31));
        targetAlignment->setMenuBar(menuBar);
        mainToolBar = new QToolBar(targetAlignment);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        targetAlignment->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(targetAlignment);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        targetAlignment->setStatusBar(statusBar);

        retranslateUi(targetAlignment);

        QMetaObject::connectSlotsByName(targetAlignment);
    } // setupUi

    void retranslateUi(QMainWindow *targetAlignment)
    {
        targetAlignment->setWindowTitle(QApplication::translate("targetAlignment", "targetAlignment", 0));
        startSensingButton->setText(QApplication::translate("targetAlignment", "Start Reading Sensors", 0));
        numberOfSensorValuesReceivedLabel->setText(QApplication::translate("targetAlignment", "Number of Sensor Values Received", 0));
        IR_0Label->setText(QApplication::translate("targetAlignment", "IR_0:", 0));
        IR_0CountLabel->setText(QApplication::translate("targetAlignment", "0", 0));
        IR_1Label->setText(QApplication::translate("targetAlignment", "IR_1:", 0));
        IR_1CountLabel->setText(QApplication::translate("targetAlignment", "0", 0));
        IR_2Label->setText(QApplication::translate("targetAlignment", "IR_2:", 0));
        IR_2CountLabel->setText(QApplication::translate("targetAlignment", "0", 0));
        label->setText(QApplication::translate("targetAlignment", "IR_0", 0));
        label_2->setText(QApplication::translate("targetAlignment", "IR_1", 0));
        label_3->setText(QApplication::translate("targetAlignment", "IR_2", 0));
        ipAddressLabel->setText(QApplication::translate("targetAlignment", "IP Address:", 0));
        serverIPLineEdit->setText(QApplication::translate("targetAlignment", "192.168.0.197", 0));
        portLabel->setText(QApplication::translate("targetAlignment", "Port: ", 0));
        portLineEdit->setText(QApplication::translate("targetAlignment", "2000", 0));
        connectedLabel->setText(QApplication::translate("targetAlignment", "Connected:", 0));
        connectionStatusLabel->setText(QApplication::translate("targetAlignment", "No", 0));
        connectToServerButton->setText(QApplication::translate("targetAlignment", "Connect To Server", 0));
        disconnectFromServerButton->setText(QApplication::translate("targetAlignment", "Disconnect From Server", 0));
    } // retranslateUi

};

namespace Ui {
    class targetAlignment: public Ui_targetAlignment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TARGETALIGNMENT_H
