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
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_targetAlignment
{
public:
    QWidget *centralWidget;
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
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QPushButton *startSensingButton;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_5;
    QLabel *numberOfSensorValuesReceivedLabel;
    QHBoxLayout *horizontalLayout_2;
    QLabel *IR_0Label;
    QLabel *IR_0CountLabel;
    QLabel *IR_1Label;
    QLabel *IR_1CountLabel;
    QLabel *IR_2Label;
    QLabel *IR_2CountLabel;
    QHBoxLayout *horizontalLayout_3;
    QLabel *GrabberIsAlignedLabel;
    QLabel *Grabber_AlignedLabel;
    QWidget *widget1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_10;
    QLabel *label;
    QSpacerItem *horizontalSpacer_7;
    QLabel *IR_0_boolLabel;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_8;
    QLabel *IR_1_boolLabel;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_9;
    QLabel *IR_2_boolLabel;
    QSpacerItem *horizontalSpacer_11;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *IR_0PlainTextEdit;
    QTextEdit *IR_0_bool_textEdit;
    QSpacerItem *horizontalSpacer_3;
    QPlainTextEdit *IR_1PlainTextEdit;
    QTextEdit *IR_1_bool_textEdit;
    QSpacerItem *horizontalSpacer_4;
    QPlainTextEdit *IR_2PlainTextEdit;
    QTextEdit *IR_2_bool_textEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *targetAlignment)
    {
        if (targetAlignment->objectName().isEmpty())
            targetAlignment->setObjectName(QStringLiteral("targetAlignment"));
        targetAlignment->resize(563, 389);
        centralWidget = new QWidget(targetAlignment);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
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

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

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

        horizontalSpacer = new QSpacerItem(69, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

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

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(351, 7, 173, 131));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        startSensingButton = new QPushButton(widget);
        startSensingButton->setObjectName(QStringLiteral("startSensingButton"));

        verticalLayout_2->addWidget(startSensingButton);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        numberOfSensorValuesReceivedLabel = new QLabel(widget);
        numberOfSensorValuesReceivedLabel->setObjectName(QStringLiteral("numberOfSensorValuesReceivedLabel"));
        numberOfSensorValuesReceivedLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(numberOfSensorValuesReceivedLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        IR_0Label = new QLabel(widget);
        IR_0Label->setObjectName(QStringLiteral("IR_0Label"));

        horizontalLayout_2->addWidget(IR_0Label);

        IR_0CountLabel = new QLabel(widget);
        IR_0CountLabel->setObjectName(QStringLiteral("IR_0CountLabel"));

        horizontalLayout_2->addWidget(IR_0CountLabel);

        IR_1Label = new QLabel(widget);
        IR_1Label->setObjectName(QStringLiteral("IR_1Label"));

        horizontalLayout_2->addWidget(IR_1Label);

        IR_1CountLabel = new QLabel(widget);
        IR_1CountLabel->setObjectName(QStringLiteral("IR_1CountLabel"));

        horizontalLayout_2->addWidget(IR_1CountLabel);

        IR_2Label = new QLabel(widget);
        IR_2Label->setObjectName(QStringLiteral("IR_2Label"));

        horizontalLayout_2->addWidget(IR_2Label);

        IR_2CountLabel = new QLabel(widget);
        IR_2CountLabel->setObjectName(QStringLiteral("IR_2CountLabel"));

        horizontalLayout_2->addWidget(IR_2CountLabel);


        verticalLayout_5->addLayout(horizontalLayout_2);


        verticalLayout_7->addLayout(verticalLayout_5);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        GrabberIsAlignedLabel = new QLabel(widget);
        GrabberIsAlignedLabel->setObjectName(QStringLiteral("GrabberIsAlignedLabel"));

        horizontalLayout_3->addWidget(GrabberIsAlignedLabel);

        Grabber_AlignedLabel = new QLabel(widget);
        Grabber_AlignedLabel->setObjectName(QStringLiteral("Grabber_AlignedLabel"));

        horizontalLayout_3->addWidget(Grabber_AlignedLabel);


        verticalLayout_7->addLayout(horizontalLayout_3);


        verticalLayout_2->addLayout(verticalLayout_7);

        widget1 = new QWidget(centralWidget);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(10, 140, 511, 181));
        verticalLayout = new QVBoxLayout(widget1);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_10 = new QSpacerItem(6, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        label = new QLabel(widget1);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label);

        horizontalSpacer_7 = new QSpacerItem(26, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        IR_0_boolLabel = new QLabel(widget1);
        IR_0_boolLabel->setObjectName(QStringLiteral("IR_0_boolLabel"));

        horizontalLayout_4->addWidget(IR_0_boolLabel);

        horizontalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        label_2 = new QLabel(widget1);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_2);

        horizontalSpacer_8 = new QSpacerItem(24, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        IR_1_boolLabel = new QLabel(widget1);
        IR_1_boolLabel->setObjectName(QStringLiteral("IR_1_boolLabel"));

        horizontalLayout_4->addWidget(IR_1_boolLabel);

        horizontalSpacer_6 = new QSpacerItem(24, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        label_3 = new QLabel(widget1);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_3);

        horizontalSpacer_9 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);

        IR_2_boolLabel = new QLabel(widget1);
        IR_2_boolLabel->setObjectName(QStringLiteral("IR_2_boolLabel"));

        horizontalLayout_4->addWidget(IR_2_boolLabel);

        horizontalSpacer_11 = new QSpacerItem(9, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_11);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        IR_0PlainTextEdit = new QPlainTextEdit(widget1);
        IR_0PlainTextEdit->setObjectName(QStringLiteral("IR_0PlainTextEdit"));

        horizontalLayout->addWidget(IR_0PlainTextEdit);

        IR_0_bool_textEdit = new QTextEdit(widget1);
        IR_0_bool_textEdit->setObjectName(QStringLiteral("IR_0_bool_textEdit"));

        horizontalLayout->addWidget(IR_0_bool_textEdit);

        horizontalSpacer_3 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        IR_1PlainTextEdit = new QPlainTextEdit(widget1);
        IR_1PlainTextEdit->setObjectName(QStringLiteral("IR_1PlainTextEdit"));

        horizontalLayout->addWidget(IR_1PlainTextEdit);

        IR_1_bool_textEdit = new QTextEdit(widget1);
        IR_1_bool_textEdit->setObjectName(QStringLiteral("IR_1_bool_textEdit"));

        horizontalLayout->addWidget(IR_1_bool_textEdit);

        horizontalSpacer_4 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        IR_2PlainTextEdit = new QPlainTextEdit(widget1);
        IR_2PlainTextEdit->setObjectName(QStringLiteral("IR_2PlainTextEdit"));

        horizontalLayout->addWidget(IR_2PlainTextEdit);

        IR_2_bool_textEdit = new QTextEdit(widget1);
        IR_2_bool_textEdit->setObjectName(QStringLiteral("IR_2_bool_textEdit"));

        horizontalLayout->addWidget(IR_2_bool_textEdit);


        verticalLayout->addLayout(horizontalLayout);

        targetAlignment->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(targetAlignment);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 563, 21));
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
        ipAddressLabel->setText(QApplication::translate("targetAlignment", "IP Address:", 0));
        serverIPLineEdit->setText(QApplication::translate("targetAlignment", "192.168.0.197", 0));
        portLabel->setText(QApplication::translate("targetAlignment", "Port: ", 0));
        portLineEdit->setText(QApplication::translate("targetAlignment", "2000", 0));
        connectedLabel->setText(QApplication::translate("targetAlignment", "Connected:", 0));
        connectionStatusLabel->setText(QApplication::translate("targetAlignment", "No", 0));
        connectToServerButton->setText(QApplication::translate("targetAlignment", "Connect To Server", 0));
        disconnectFromServerButton->setText(QApplication::translate("targetAlignment", "Disconnect From Server", 0));
        startSensingButton->setText(QApplication::translate("targetAlignment", "Start Reading Sensors", 0));
        numberOfSensorValuesReceivedLabel->setText(QApplication::translate("targetAlignment", "Number of Sensor Values Received", 0));
        IR_0Label->setText(QApplication::translate("targetAlignment", "IR_0:", 0));
        IR_0CountLabel->setText(QApplication::translate("targetAlignment", "0", 0));
        IR_1Label->setText(QApplication::translate("targetAlignment", "IR_1:", 0));
        IR_1CountLabel->setText(QApplication::translate("targetAlignment", "0", 0));
        IR_2Label->setText(QApplication::translate("targetAlignment", "IR_2:", 0));
        IR_2CountLabel->setText(QApplication::translate("targetAlignment", "0", 0));
        GrabberIsAlignedLabel->setText(QApplication::translate("targetAlignment", "Grabber is Aligned:", 0));
        Grabber_AlignedLabel->setText(QApplication::translate("targetAlignment", "No", 0));
        label->setText(QApplication::translate("targetAlignment", "IR_0", 0));
        IR_0_boolLabel->setText(QApplication::translate("targetAlignment", "IR_0_bool", 0));
        label_2->setText(QApplication::translate("targetAlignment", "IR_1", 0));
        IR_1_boolLabel->setText(QApplication::translate("targetAlignment", "IR_1_bool", 0));
        label_3->setText(QApplication::translate("targetAlignment", "IR_2", 0));
        IR_2_boolLabel->setText(QApplication::translate("targetAlignment", "IR_2_bool", 0));
    } // retranslateUi

};

namespace Ui {
    class targetAlignment: public Ui_targetAlignment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TARGETALIGNMENT_H
