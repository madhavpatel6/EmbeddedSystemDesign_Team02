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
#include <QtWidgets/QLCDNumber>
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
    QWidget *layoutWidget;
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
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QPushButton *startSensingButton;
    QLabel *numberOfSensorValuesReceivedLabel;
    QHBoxLayout *horizontalLayout_3;
    QLabel *IR_0Label;
    QLabel *IR_0CountLabel;
    QLabel *IR_1Label;
    QLabel *IR_1CountLabel;
    QLabel *IR_2Label;
    QLabel *IR_2CountLabel;
    QHBoxLayout *horizontalLayout_2;
    QLabel *GrabberIsAlignedLabel;
    QLabel *Grabber_AlignedLabel;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_5;
    QLabel *AlignmentInfoLabel;
    QLabel *Alignment_InfoLabel;
    QHBoxLayout *horizontalLayout_11;
    QVBoxLayout *verticalLayout_8;
    QPushButton *calculateVarianceButton;
    QHBoxLayout *horizontalLayout_10;
    QLabel *varianceStartedLabel;
    QLabel *variance_started_label;
    QVBoxLayout *verticalLayout_7;
    QLabel *varianceIR0Label;
    QLCDNumber *varianceIR0LcdNumber;
    QVBoxLayout *verticalLayout_5;
    QLabel *varianceIR1Label;
    QLCDNumber *varianceIR1LcdNumber;
    QVBoxLayout *verticalLayout_3;
    QLabel *varianceIR2Label;
    QLCDNumber *varianceIR2LcdNumber;
    QWidget *widget;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_4;
    QLCDNumber *meanIR0LcdNumber;
    QLCDNumber *meanIR1LcdNumber;
    QLCDNumber *meanIR2LcdNumber;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_12;
    QLabel *xiLabel;
    QLCDNumber *ximeanIR0LcdNumber;
    QLCDNumber *ximeanIR1LcdNumber;
    QLCDNumber *ximeanIR2LcdNumber;
    QHBoxLayout *horizontalLayout_13;
    QLabel *ximeansqLabel;
    QLCDNumber *ximeansqIR0LcdNumber;
    QLCDNumber *ximeansqIR1LcdNumber;
    QLCDNumber *ximeansqIR2LcdNumber;
    QHBoxLayout *horizontalLayout_14;
    QLabel *sumximeansqLabel;
    QLCDNumber *sumximeansqIR0LcdNumber;
    QLCDNumber *sumximeansqIR1LcdNumber;
    QLCDNumber *sumximeansqIR2LcdNumber;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *targetAlignment)
    {
        if (targetAlignment->objectName().isEmpty())
            targetAlignment->setObjectName(QStringLiteral("targetAlignment"));
        targetAlignment->resize(802, 625);
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

        horizontalSpacer_2 = new QSpacerItem(48, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

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

        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 140, 681, 181));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_10 = new QSpacerItem(6, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label);

        horizontalSpacer_7 = new QSpacerItem(47, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        IR_0_boolLabel = new QLabel(layoutWidget);
        IR_0_boolLabel->setObjectName(QStringLiteral("IR_0_boolLabel"));

        horizontalLayout_4->addWidget(IR_0_boolLabel);

        horizontalSpacer_5 = new QSpacerItem(11, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_2);

        horizontalSpacer_8 = new QSpacerItem(56, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        IR_1_boolLabel = new QLabel(layoutWidget);
        IR_1_boolLabel->setObjectName(QStringLiteral("IR_1_boolLabel"));

        horizontalLayout_4->addWidget(IR_1_boolLabel);

        horizontalSpacer_6 = new QSpacerItem(24, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_3);

        horizontalSpacer_9 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);

        IR_2_boolLabel = new QLabel(layoutWidget);
        IR_2_boolLabel->setObjectName(QStringLiteral("IR_2_boolLabel"));

        horizontalLayout_4->addWidget(IR_2_boolLabel);

        horizontalSpacer_11 = new QSpacerItem(9, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_11);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        IR_0PlainTextEdit = new QPlainTextEdit(layoutWidget);
        IR_0PlainTextEdit->setObjectName(QStringLiteral("IR_0PlainTextEdit"));

        horizontalLayout->addWidget(IR_0PlainTextEdit);

        IR_0_bool_textEdit = new QTextEdit(layoutWidget);
        IR_0_bool_textEdit->setObjectName(QStringLiteral("IR_0_bool_textEdit"));

        horizontalLayout->addWidget(IR_0_bool_textEdit);

        horizontalSpacer_3 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        IR_1PlainTextEdit = new QPlainTextEdit(layoutWidget);
        IR_1PlainTextEdit->setObjectName(QStringLiteral("IR_1PlainTextEdit"));

        horizontalLayout->addWidget(IR_1PlainTextEdit);

        IR_1_bool_textEdit = new QTextEdit(layoutWidget);
        IR_1_bool_textEdit->setObjectName(QStringLiteral("IR_1_bool_textEdit"));

        horizontalLayout->addWidget(IR_1_bool_textEdit);

        horizontalSpacer_4 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        IR_2PlainTextEdit = new QPlainTextEdit(layoutWidget);
        IR_2PlainTextEdit->setObjectName(QStringLiteral("IR_2PlainTextEdit"));

        horizontalLayout->addWidget(IR_2PlainTextEdit);

        IR_2_bool_textEdit = new QTextEdit(layoutWidget);
        IR_2_bool_textEdit->setObjectName(QStringLiteral("IR_2_bool_textEdit"));

        horizontalLayout->addWidget(IR_2_bool_textEdit);


        verticalLayout->addLayout(horizontalLayout);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(352, 10, 341, 131));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        startSensingButton = new QPushButton(layoutWidget1);
        startSensingButton->setObjectName(QStringLiteral("startSensingButton"));

        verticalLayout_2->addWidget(startSensingButton);

        numberOfSensorValuesReceivedLabel = new QLabel(layoutWidget1);
        numberOfSensorValuesReceivedLabel->setObjectName(QStringLiteral("numberOfSensorValuesReceivedLabel"));
        numberOfSensorValuesReceivedLabel->setAlignment(Qt::AlignCenter);
        numberOfSensorValuesReceivedLabel->setWordWrap(false);

        verticalLayout_2->addWidget(numberOfSensorValuesReceivedLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        IR_0Label = new QLabel(layoutWidget1);
        IR_0Label->setObjectName(QStringLiteral("IR_0Label"));

        horizontalLayout_3->addWidget(IR_0Label);

        IR_0CountLabel = new QLabel(layoutWidget1);
        IR_0CountLabel->setObjectName(QStringLiteral("IR_0CountLabel"));

        horizontalLayout_3->addWidget(IR_0CountLabel);

        IR_1Label = new QLabel(layoutWidget1);
        IR_1Label->setObjectName(QStringLiteral("IR_1Label"));

        horizontalLayout_3->addWidget(IR_1Label);

        IR_1CountLabel = new QLabel(layoutWidget1);
        IR_1CountLabel->setObjectName(QStringLiteral("IR_1CountLabel"));

        horizontalLayout_3->addWidget(IR_1CountLabel);

        IR_2Label = new QLabel(layoutWidget1);
        IR_2Label->setObjectName(QStringLiteral("IR_2Label"));

        horizontalLayout_3->addWidget(IR_2Label);

        IR_2CountLabel = new QLabel(layoutWidget1);
        IR_2CountLabel->setObjectName(QStringLiteral("IR_2CountLabel"));

        horizontalLayout_3->addWidget(IR_2CountLabel);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        GrabberIsAlignedLabel = new QLabel(layoutWidget1);
        GrabberIsAlignedLabel->setObjectName(QStringLiteral("GrabberIsAlignedLabel"));

        horizontalLayout_2->addWidget(GrabberIsAlignedLabel);

        Grabber_AlignedLabel = new QLabel(layoutWidget1);
        Grabber_AlignedLabel->setObjectName(QStringLiteral("Grabber_AlignedLabel"));

        horizontalLayout_2->addWidget(Grabber_AlignedLabel);


        verticalLayout_2->addLayout(horizontalLayout_2);

        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 332, 681, 86));
        verticalLayout_9 = new QVBoxLayout(layoutWidget2);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        AlignmentInfoLabel = new QLabel(layoutWidget2);
        AlignmentInfoLabel->setObjectName(QStringLiteral("AlignmentInfoLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AlignmentInfoLabel->sizePolicy().hasHeightForWidth());
        AlignmentInfoLabel->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(AlignmentInfoLabel);

        Alignment_InfoLabel = new QLabel(layoutWidget2);
        Alignment_InfoLabel->setObjectName(QStringLiteral("Alignment_InfoLabel"));
        Alignment_InfoLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(Alignment_InfoLabel);


        verticalLayout_9->addLayout(horizontalLayout_5);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        calculateVarianceButton = new QPushButton(layoutWidget2);
        calculateVarianceButton->setObjectName(QStringLiteral("calculateVarianceButton"));

        verticalLayout_8->addWidget(calculateVarianceButton);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        varianceStartedLabel = new QLabel(layoutWidget2);
        varianceStartedLabel->setObjectName(QStringLiteral("varianceStartedLabel"));
        sizePolicy.setHeightForWidth(varianceStartedLabel->sizePolicy().hasHeightForWidth());
        varianceStartedLabel->setSizePolicy(sizePolicy);

        horizontalLayout_10->addWidget(varianceStartedLabel);

        variance_started_label = new QLabel(layoutWidget2);
        variance_started_label->setObjectName(QStringLiteral("variance_started_label"));

        horizontalLayout_10->addWidget(variance_started_label);


        verticalLayout_8->addLayout(horizontalLayout_10);


        horizontalLayout_11->addLayout(verticalLayout_8);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        varianceIR0Label = new QLabel(layoutWidget2);
        varianceIR0Label->setObjectName(QStringLiteral("varianceIR0Label"));
        varianceIR0Label->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(varianceIR0Label);

        varianceIR0LcdNumber = new QLCDNumber(layoutWidget2);
        varianceIR0LcdNumber->setObjectName(QStringLiteral("varianceIR0LcdNumber"));

        verticalLayout_7->addWidget(varianceIR0LcdNumber);


        horizontalLayout_11->addLayout(verticalLayout_7);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        varianceIR1Label = new QLabel(layoutWidget2);
        varianceIR1Label->setObjectName(QStringLiteral("varianceIR1Label"));
        varianceIR1Label->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(varianceIR1Label);

        varianceIR1LcdNumber = new QLCDNumber(layoutWidget2);
        varianceIR1LcdNumber->setObjectName(QStringLiteral("varianceIR1LcdNumber"));

        verticalLayout_5->addWidget(varianceIR1LcdNumber);


        horizontalLayout_11->addLayout(verticalLayout_5);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        varianceIR2Label = new QLabel(layoutWidget2);
        varianceIR2Label->setObjectName(QStringLiteral("varianceIR2Label"));
        varianceIR2Label->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(varianceIR2Label);

        varianceIR2LcdNumber = new QLCDNumber(layoutWidget2);
        varianceIR2LcdNumber->setObjectName(QStringLiteral("varianceIR2LcdNumber"));

        verticalLayout_3->addWidget(varianceIR2LcdNumber);


        horizontalLayout_11->addLayout(verticalLayout_3);


        verticalLayout_9->addLayout(horizontalLayout_11);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 420, 681, 125));
        verticalLayout_11 = new QVBoxLayout(widget);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_15->addWidget(label_4);

        meanIR0LcdNumber = new QLCDNumber(widget);
        meanIR0LcdNumber->setObjectName(QStringLiteral("meanIR0LcdNumber"));

        horizontalLayout_15->addWidget(meanIR0LcdNumber);

        meanIR1LcdNumber = new QLCDNumber(widget);
        meanIR1LcdNumber->setObjectName(QStringLiteral("meanIR1LcdNumber"));

        horizontalLayout_15->addWidget(meanIR1LcdNumber);

        meanIR2LcdNumber = new QLCDNumber(widget);
        meanIR2LcdNumber->setObjectName(QStringLiteral("meanIR2LcdNumber"));

        horizontalLayout_15->addWidget(meanIR2LcdNumber);


        verticalLayout_11->addLayout(horizontalLayout_15);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        xiLabel = new QLabel(widget);
        xiLabel->setObjectName(QStringLiteral("xiLabel"));

        horizontalLayout_12->addWidget(xiLabel);

        ximeanIR0LcdNumber = new QLCDNumber(widget);
        ximeanIR0LcdNumber->setObjectName(QStringLiteral("ximeanIR0LcdNumber"));

        horizontalLayout_12->addWidget(ximeanIR0LcdNumber);

        ximeanIR1LcdNumber = new QLCDNumber(widget);
        ximeanIR1LcdNumber->setObjectName(QStringLiteral("ximeanIR1LcdNumber"));

        horizontalLayout_12->addWidget(ximeanIR1LcdNumber);

        ximeanIR2LcdNumber = new QLCDNumber(widget);
        ximeanIR2LcdNumber->setObjectName(QStringLiteral("ximeanIR2LcdNumber"));

        horizontalLayout_12->addWidget(ximeanIR2LcdNumber);


        verticalLayout_10->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        ximeansqLabel = new QLabel(widget);
        ximeansqLabel->setObjectName(QStringLiteral("ximeansqLabel"));

        horizontalLayout_13->addWidget(ximeansqLabel);

        ximeansqIR0LcdNumber = new QLCDNumber(widget);
        ximeansqIR0LcdNumber->setObjectName(QStringLiteral("ximeansqIR0LcdNumber"));

        horizontalLayout_13->addWidget(ximeansqIR0LcdNumber);

        ximeansqIR1LcdNumber = new QLCDNumber(widget);
        ximeansqIR1LcdNumber->setObjectName(QStringLiteral("ximeansqIR1LcdNumber"));

        horizontalLayout_13->addWidget(ximeansqIR1LcdNumber);

        ximeansqIR2LcdNumber = new QLCDNumber(widget);
        ximeansqIR2LcdNumber->setObjectName(QStringLiteral("ximeansqIR2LcdNumber"));

        horizontalLayout_13->addWidget(ximeansqIR2LcdNumber);


        verticalLayout_10->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        sumximeansqLabel = new QLabel(widget);
        sumximeansqLabel->setObjectName(QStringLiteral("sumximeansqLabel"));

        horizontalLayout_14->addWidget(sumximeansqLabel);

        sumximeansqIR0LcdNumber = new QLCDNumber(widget);
        sumximeansqIR0LcdNumber->setObjectName(QStringLiteral("sumximeansqIR0LcdNumber"));

        horizontalLayout_14->addWidget(sumximeansqIR0LcdNumber);

        sumximeansqIR1LcdNumber = new QLCDNumber(widget);
        sumximeansqIR1LcdNumber->setObjectName(QStringLiteral("sumximeansqIR1LcdNumber"));

        horizontalLayout_14->addWidget(sumximeansqIR1LcdNumber);

        sumximeansqIR2LcdNumber = new QLCDNumber(widget);
        sumximeansqIR2LcdNumber->setObjectName(QStringLiteral("sumximeansqIR2LcdNumber"));

        horizontalLayout_14->addWidget(sumximeansqIR2LcdNumber);


        verticalLayout_10->addLayout(horizontalLayout_14);


        verticalLayout_11->addLayout(verticalLayout_10);

        targetAlignment->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(targetAlignment);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 802, 26));
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
        serverIPLineEdit->setText(QApplication::translate("targetAlignment", "192.168.0.5", 0));
        portLabel->setText(QApplication::translate("targetAlignment", "Port: ", 0));
        portLineEdit->setText(QApplication::translate("targetAlignment", "2000", 0));
        connectedLabel->setText(QApplication::translate("targetAlignment", "Connected:", 0));
        connectionStatusLabel->setText(QApplication::translate("targetAlignment", "No", 0));
        connectToServerButton->setText(QApplication::translate("targetAlignment", "Connect To Server", 0));
        disconnectFromServerButton->setText(QApplication::translate("targetAlignment", "Disconnect From Server", 0));
        label->setText(QApplication::translate("targetAlignment", "IR_0", 0));
        IR_0_boolLabel->setText(QApplication::translate("targetAlignment", "IR_0_bool", 0));
        label_2->setText(QApplication::translate("targetAlignment", "IR_1", 0));
        IR_1_boolLabel->setText(QApplication::translate("targetAlignment", "IR_1_bool", 0));
        label_3->setText(QApplication::translate("targetAlignment", "IR_2", 0));
        IR_2_boolLabel->setText(QApplication::translate("targetAlignment", "IR_2_bool", 0));
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
        AlignmentInfoLabel->setText(QApplication::translate("targetAlignment", "Alignment Information:", 0));
        Alignment_InfoLabel->setText(QApplication::translate("targetAlignment", "N/A", 0));
        calculateVarianceButton->setText(QApplication::translate("targetAlignment", "Calclaute Variance", 0));
        varianceStartedLabel->setText(QApplication::translate("targetAlignment", "Variance Started:", 0));
        variance_started_label->setText(QApplication::translate("targetAlignment", "No", 0));
        varianceIR0Label->setText(QApplication::translate("targetAlignment", "Variance IR_0:", 0));
        varianceIR1Label->setText(QApplication::translate("targetAlignment", "Variance IR_1:", 0));
        varianceIR2Label->setText(QApplication::translate("targetAlignment", "Variance IR_2:", 0));
        label_4->setText(QApplication::translate("targetAlignment", "Mean:", 0));
        xiLabel->setText(QApplication::translate("targetAlignment", "Xi-Mean:", 0));
        ximeansqLabel->setText(QApplication::translate("targetAlignment", "(Xi-Mean)^2 :", 0));
        sumximeansqLabel->setText(QApplication::translate("targetAlignment", "Sum (Xi-Mean)^2:", 0));
    } // retranslateUi

};

namespace Ui {
    class targetAlignment: public Ui_targetAlignment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TARGETALIGNMENT_H
