/********************************************************************************
** Form generated from reading UI file 'pfwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PFWINDOW_H
#define UI_PFWINDOW_H

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
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_pfWindow
{
public:
    QAction *actionCalculate_Variance;
    QAction *actionRequests_Responses;
    QAction *actionTG_Requests_and_Responses;
    QWidget *centralWidget;
    QWidget *layoutWidget_4;
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
    QLabel *IR_3label;
    QLabel *IR_3CountLabel;
    QLabel *IR_4label;
    QLabel *IR_4CountLabel;
    QHBoxLayout *horizontalLayout_2;
    QLabel *GrabberIsAlignedLabel;
    QLabel *Grabber_AlignedLabel;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_18;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPlainTextEdit *IR_0PlainTextEdit;
    QVBoxLayout *verticalLayout_8;
    QLabel *IR_0_boolLabel;
    QTextEdit *IR_0_bool_textEdit;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_2;
    QPlainTextEdit *IR_1PlainTextEdit;
    QVBoxLayout *verticalLayout_10;
    QLabel *IR_1_boolLabel;
    QTextEdit *IR_1_bool_textEdit;
    QVBoxLayout *verticalLayout_11;
    QLabel *label_3;
    QPlainTextEdit *IR_2PlainTextEdit;
    QVBoxLayout *verticalLayout_12;
    QLabel *IR_2_boolLabel;
    QTextEdit *IR_2_bool_textEdit;
    QVBoxLayout *verticalLayout_13;
    QLabel *IR_3collabel;
    QPlainTextEdit *IR_3PlainTextEdit;
    QVBoxLayout *verticalLayout_14;
    QLabel *IR_3label_2;
    QTextEdit *IR_3_bool_textEdit;
    QVBoxLayout *verticalLayout_15;
    QLabel *IR_4collabel;
    QPlainTextEdit *IR_4PlainTextEdit;
    QVBoxLayout *verticalLayout_16;
    QLabel *IR_3label_3;
    QTextEdit *IR_4_bool_textEdit;
    QVBoxLayout *verticalLayout_17;
    QHBoxLayout *horizontalLayout_13;
    QHBoxLayout *horizontalLayout_5;
    QLabel *AlignmentInfoLabel;
    QLabel *Alignment_InfoLabel;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *proximityPushButton;
    QHBoxLayout *horizontalLayout_10;
    QLabel *proxsentboollabel;
    QLabel *prox_sent_bool_label;
    QHBoxLayout *horizontalLayout_11;
    QLabel *proximityValueLabel;
    QSlider *proximitySlider;
    QLCDNumber *proxlcdNumber;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_18;
    QPushButton *sendMovementPushButton;
    QHBoxLayout *horizontalLayout_17;
    QLabel *movementBoollabel;
    QLabel *movement_bool_label;
    QHBoxLayout *horizontalLayout_19;
    QPushButton *sendOutsideofArenaPushButton;
    QHBoxLayout *horizontalLayout_16;
    QLabel *outsideofArenaBoollabel;
    QLabel *outside_of_arena_bool_label;
    QHBoxLayout *horizontalLayout_20;
    QPushButton *requestTargetAcquiredpushButton;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_15;
    QLabel *targetAcquiredlabel;
    QLabel *target_acquired_bool_label;
    QHBoxLayout *horizontalLayout_14;
    QLabel *targetAcquiredReqSentBoollabel;
    QLabel *target_Acquired_Request_Sent_Bool_label;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_22;
    QHBoxLayout *horizontalLayout_21;
    QLabel *expectedDegreeLabel;
    QLCDNumber *angleLcdNumber;
    QHBoxLayout *horizontalLayout_4;
    QLabel *DistanceFromTargetLabel;
    QLCDNumber *distanceFromTargetLcdNumber;
    QMenuBar *menuBar;
    QMenu *menuCalculate_Variance;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *pfWindow)
    {
        if (pfWindow->objectName().isEmpty())
            pfWindow->setObjectName(QStringLiteral("pfWindow"));
        pfWindow->resize(1162, 693);
        actionCalculate_Variance = new QAction(pfWindow);
        actionCalculate_Variance->setObjectName(QStringLiteral("actionCalculate_Variance"));
        actionRequests_Responses = new QAction(pfWindow);
        actionRequests_Responses->setObjectName(QStringLiteral("actionRequests_Responses"));
        actionTG_Requests_and_Responses = new QAction(pfWindow);
        actionTG_Requests_and_Responses->setObjectName(QStringLiteral("actionTG_Requests_and_Responses"));
        centralWidget = new QWidget(pfWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget_4 = new QWidget(centralWidget);
        layoutWidget_4->setObjectName(QStringLiteral("layoutWidget_4"));
        layoutWidget_4->setGeometry(QRect(9, 7, 339, 134));
        verticalLayout_6 = new QVBoxLayout(layoutWidget_4);
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
        ipAddressLabel = new QLabel(layoutWidget_4);
        ipAddressLabel->setObjectName(QStringLiteral("ipAddressLabel"));

        horizontalLayout_8->addWidget(ipAddressLabel);

        horizontalSpacer_2 = new QSpacerItem(48, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);

        serverIPLineEdit = new QLineEdit(layoutWidget_4);
        serverIPLineEdit->setObjectName(QStringLiteral("serverIPLineEdit"));

        horizontalLayout_8->addWidget(serverIPLineEdit);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        portLabel = new QLabel(layoutWidget_4);
        portLabel->setObjectName(QStringLiteral("portLabel"));

        horizontalLayout_9->addWidget(portLabel);

        horizontalSpacer = new QSpacerItem(69, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer);

        portLineEdit = new QLineEdit(layoutWidget_4);
        portLineEdit->setObjectName(QStringLiteral("portLineEdit"));

        horizontalLayout_9->addWidget(portLineEdit);


        verticalLayout_4->addLayout(horizontalLayout_9);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        connectedLabel = new QLabel(layoutWidget_4);
        connectedLabel->setObjectName(QStringLiteral("connectedLabel"));

        horizontalLayout_6->addWidget(connectedLabel);

        connectionStatusLabel = new QLabel(layoutWidget_4);
        connectionStatusLabel->setObjectName(QStringLiteral("connectionStatusLabel"));

        horizontalLayout_6->addWidget(connectionStatusLabel);


        verticalLayout_4->addLayout(horizontalLayout_6);


        verticalLayout_6->addLayout(verticalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        connectToServerButton = new QPushButton(layoutWidget_4);
        connectToServerButton->setObjectName(QStringLiteral("connectToServerButton"));

        horizontalLayout_7->addWidget(connectToServerButton);

        disconnectFromServerButton = new QPushButton(layoutWidget_4);
        disconnectFromServerButton->setObjectName(QStringLiteral("disconnectFromServerButton"));

        horizontalLayout_7->addWidget(disconnectFromServerButton);


        verticalLayout_6->addLayout(horizontalLayout_7);

        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(353, 11, 791, 131));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        startSensingButton = new QPushButton(layoutWidget);
        startSensingButton->setObjectName(QStringLiteral("startSensingButton"));

        verticalLayout_2->addWidget(startSensingButton);

        numberOfSensorValuesReceivedLabel = new QLabel(layoutWidget);
        numberOfSensorValuesReceivedLabel->setObjectName(QStringLiteral("numberOfSensorValuesReceivedLabel"));
        numberOfSensorValuesReceivedLabel->setAlignment(Qt::AlignCenter);
        numberOfSensorValuesReceivedLabel->setWordWrap(false);

        verticalLayout_2->addWidget(numberOfSensorValuesReceivedLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        IR_0Label = new QLabel(layoutWidget);
        IR_0Label->setObjectName(QStringLiteral("IR_0Label"));

        horizontalLayout_3->addWidget(IR_0Label);

        IR_0CountLabel = new QLabel(layoutWidget);
        IR_0CountLabel->setObjectName(QStringLiteral("IR_0CountLabel"));

        horizontalLayout_3->addWidget(IR_0CountLabel);

        IR_1Label = new QLabel(layoutWidget);
        IR_1Label->setObjectName(QStringLiteral("IR_1Label"));

        horizontalLayout_3->addWidget(IR_1Label);

        IR_1CountLabel = new QLabel(layoutWidget);
        IR_1CountLabel->setObjectName(QStringLiteral("IR_1CountLabel"));

        horizontalLayout_3->addWidget(IR_1CountLabel);

        IR_2Label = new QLabel(layoutWidget);
        IR_2Label->setObjectName(QStringLiteral("IR_2Label"));

        horizontalLayout_3->addWidget(IR_2Label);

        IR_2CountLabel = new QLabel(layoutWidget);
        IR_2CountLabel->setObjectName(QStringLiteral("IR_2CountLabel"));

        horizontalLayout_3->addWidget(IR_2CountLabel);

        IR_3label = new QLabel(layoutWidget);
        IR_3label->setObjectName(QStringLiteral("IR_3label"));

        horizontalLayout_3->addWidget(IR_3label);

        IR_3CountLabel = new QLabel(layoutWidget);
        IR_3CountLabel->setObjectName(QStringLiteral("IR_3CountLabel"));

        horizontalLayout_3->addWidget(IR_3CountLabel);

        IR_4label = new QLabel(layoutWidget);
        IR_4label->setObjectName(QStringLiteral("IR_4label"));

        horizontalLayout_3->addWidget(IR_4label);

        IR_4CountLabel = new QLabel(layoutWidget);
        IR_4CountLabel->setObjectName(QStringLiteral("IR_4CountLabel"));

        horizontalLayout_3->addWidget(IR_4CountLabel);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        GrabberIsAlignedLabel = new QLabel(layoutWidget);
        GrabberIsAlignedLabel->setObjectName(QStringLiteral("GrabberIsAlignedLabel"));

        horizontalLayout_2->addWidget(GrabberIsAlignedLabel);

        Grabber_AlignedLabel = new QLabel(layoutWidget);
        Grabber_AlignedLabel->setObjectName(QStringLiteral("Grabber_AlignedLabel"));

        horizontalLayout_2->addWidget(Grabber_AlignedLabel);


        verticalLayout_2->addLayout(horizontalLayout_2);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 144, 1131, 401));
        verticalLayout_18 = new QVBoxLayout(layoutWidget1);
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setContentsMargins(11, 11, 11, 11);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        verticalLayout_18->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        IR_0PlainTextEdit = new QPlainTextEdit(layoutWidget1);
        IR_0PlainTextEdit->setObjectName(QStringLiteral("IR_0PlainTextEdit"));

        verticalLayout->addWidget(IR_0PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        IR_0_boolLabel = new QLabel(layoutWidget1);
        IR_0_boolLabel->setObjectName(QStringLiteral("IR_0_boolLabel"));
        IR_0_boolLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(IR_0_boolLabel);

        IR_0_bool_textEdit = new QTextEdit(layoutWidget1);
        IR_0_bool_textEdit->setObjectName(QStringLiteral("IR_0_bool_textEdit"));

        verticalLayout_8->addWidget(IR_0_bool_textEdit);


        horizontalLayout->addLayout(verticalLayout_8);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(label_2);

        IR_1PlainTextEdit = new QPlainTextEdit(layoutWidget1);
        IR_1PlainTextEdit->setObjectName(QStringLiteral("IR_1PlainTextEdit"));

        verticalLayout_9->addWidget(IR_1PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout_9);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        IR_1_boolLabel = new QLabel(layoutWidget1);
        IR_1_boolLabel->setObjectName(QStringLiteral("IR_1_boolLabel"));
        IR_1_boolLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(IR_1_boolLabel);

        IR_1_bool_textEdit = new QTextEdit(layoutWidget1);
        IR_1_bool_textEdit->setObjectName(QStringLiteral("IR_1_bool_textEdit"));

        verticalLayout_10->addWidget(IR_1_bool_textEdit);


        horizontalLayout->addLayout(verticalLayout_10);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(label_3);

        IR_2PlainTextEdit = new QPlainTextEdit(layoutWidget1);
        IR_2PlainTextEdit->setObjectName(QStringLiteral("IR_2PlainTextEdit"));

        verticalLayout_11->addWidget(IR_2PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout_11);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        IR_2_boolLabel = new QLabel(layoutWidget1);
        IR_2_boolLabel->setObjectName(QStringLiteral("IR_2_boolLabel"));
        IR_2_boolLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(IR_2_boolLabel);

        IR_2_bool_textEdit = new QTextEdit(layoutWidget1);
        IR_2_bool_textEdit->setObjectName(QStringLiteral("IR_2_bool_textEdit"));

        verticalLayout_12->addWidget(IR_2_bool_textEdit);


        horizontalLayout->addLayout(verticalLayout_12);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        IR_3collabel = new QLabel(layoutWidget1);
        IR_3collabel->setObjectName(QStringLiteral("IR_3collabel"));
        IR_3collabel->setAlignment(Qt::AlignCenter);

        verticalLayout_13->addWidget(IR_3collabel);

        IR_3PlainTextEdit = new QPlainTextEdit(layoutWidget1);
        IR_3PlainTextEdit->setObjectName(QStringLiteral("IR_3PlainTextEdit"));

        verticalLayout_13->addWidget(IR_3PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout_13);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        IR_3label_2 = new QLabel(layoutWidget1);
        IR_3label_2->setObjectName(QStringLiteral("IR_3label_2"));
        IR_3label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(IR_3label_2);

        IR_3_bool_textEdit = new QTextEdit(layoutWidget1);
        IR_3_bool_textEdit->setObjectName(QStringLiteral("IR_3_bool_textEdit"));

        verticalLayout_14->addWidget(IR_3_bool_textEdit);


        horizontalLayout->addLayout(verticalLayout_14);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        IR_4collabel = new QLabel(layoutWidget1);
        IR_4collabel->setObjectName(QStringLiteral("IR_4collabel"));
        IR_4collabel->setAlignment(Qt::AlignCenter);

        verticalLayout_15->addWidget(IR_4collabel);

        IR_4PlainTextEdit = new QPlainTextEdit(layoutWidget1);
        IR_4PlainTextEdit->setObjectName(QStringLiteral("IR_4PlainTextEdit"));

        verticalLayout_15->addWidget(IR_4PlainTextEdit);


        horizontalLayout->addLayout(verticalLayout_15);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        IR_3label_3 = new QLabel(layoutWidget1);
        IR_3label_3->setObjectName(QStringLiteral("IR_3label_3"));
        IR_3label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_16->addWidget(IR_3label_3);

        IR_4_bool_textEdit = new QTextEdit(layoutWidget1);
        IR_4_bool_textEdit->setObjectName(QStringLiteral("IR_4_bool_textEdit"));

        verticalLayout_16->addWidget(IR_4_bool_textEdit);


        horizontalLayout->addLayout(verticalLayout_16);


        verticalLayout_18->addLayout(horizontalLayout);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        AlignmentInfoLabel = new QLabel(layoutWidget1);
        AlignmentInfoLabel->setObjectName(QStringLiteral("AlignmentInfoLabel"));

        horizontalLayout_5->addWidget(AlignmentInfoLabel);

        Alignment_InfoLabel = new QLabel(layoutWidget1);
        Alignment_InfoLabel->setObjectName(QStringLiteral("Alignment_InfoLabel"));
        Alignment_InfoLabel->setAlignment(Qt::AlignCenter);
        Alignment_InfoLabel->setWordWrap(true);

        horizontalLayout_5->addWidget(Alignment_InfoLabel);


        horizontalLayout_13->addLayout(horizontalLayout_5);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        proximityPushButton = new QPushButton(layoutWidget1);
        proximityPushButton->setObjectName(QStringLiteral("proximityPushButton"));

        horizontalLayout_12->addWidget(proximityPushButton);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        proxsentboollabel = new QLabel(layoutWidget1);
        proxsentboollabel->setObjectName(QStringLiteral("proxsentboollabel"));

        horizontalLayout_10->addWidget(proxsentboollabel);

        prox_sent_bool_label = new QLabel(layoutWidget1);
        prox_sent_bool_label->setObjectName(QStringLiteral("prox_sent_bool_label"));

        horizontalLayout_10->addWidget(prox_sent_bool_label);


        horizontalLayout_12->addLayout(horizontalLayout_10);


        verticalLayout_3->addLayout(horizontalLayout_12);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        proximityValueLabel = new QLabel(layoutWidget1);
        proximityValueLabel->setObjectName(QStringLiteral("proximityValueLabel"));

        horizontalLayout_11->addWidget(proximityValueLabel);

        proximitySlider = new QSlider(layoutWidget1);
        proximitySlider->setObjectName(QStringLiteral("proximitySlider"));
        proximitySlider->setMaximum(20);
        proximitySlider->setValue(10);
        proximitySlider->setSliderPosition(10);
        proximitySlider->setOrientation(Qt::Horizontal);

        horizontalLayout_11->addWidget(proximitySlider);

        proxlcdNumber = new QLCDNumber(layoutWidget1);
        proxlcdNumber->setObjectName(QStringLiteral("proxlcdNumber"));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        proxlcdNumber->setFont(font);
        proxlcdNumber->setAutoFillBackground(false);
        proxlcdNumber->setProperty("intValue", QVariant(10));

        horizontalLayout_11->addWidget(proxlcdNumber);


        verticalLayout_3->addLayout(horizontalLayout_11);


        horizontalLayout_13->addLayout(verticalLayout_3);


        verticalLayout_17->addLayout(horizontalLayout_13);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        sendMovementPushButton = new QPushButton(layoutWidget1);
        sendMovementPushButton->setObjectName(QStringLiteral("sendMovementPushButton"));

        horizontalLayout_18->addWidget(sendMovementPushButton);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        movementBoollabel = new QLabel(layoutWidget1);
        movementBoollabel->setObjectName(QStringLiteral("movementBoollabel"));

        horizontalLayout_17->addWidget(movementBoollabel);

        movement_bool_label = new QLabel(layoutWidget1);
        movement_bool_label->setObjectName(QStringLiteral("movement_bool_label"));

        horizontalLayout_17->addWidget(movement_bool_label);


        horizontalLayout_18->addLayout(horizontalLayout_17);


        verticalLayout_7->addLayout(horizontalLayout_18);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        sendOutsideofArenaPushButton = new QPushButton(layoutWidget1);
        sendOutsideofArenaPushButton->setObjectName(QStringLiteral("sendOutsideofArenaPushButton"));

        horizontalLayout_19->addWidget(sendOutsideofArenaPushButton);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        outsideofArenaBoollabel = new QLabel(layoutWidget1);
        outsideofArenaBoollabel->setObjectName(QStringLiteral("outsideofArenaBoollabel"));

        horizontalLayout_16->addWidget(outsideofArenaBoollabel);

        outside_of_arena_bool_label = new QLabel(layoutWidget1);
        outside_of_arena_bool_label->setObjectName(QStringLiteral("outside_of_arena_bool_label"));

        horizontalLayout_16->addWidget(outside_of_arena_bool_label);


        horizontalLayout_19->addLayout(horizontalLayout_16);


        verticalLayout_7->addLayout(horizontalLayout_19);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        requestTargetAcquiredpushButton = new QPushButton(layoutWidget1);
        requestTargetAcquiredpushButton->setObjectName(QStringLiteral("requestTargetAcquiredpushButton"));

        horizontalLayout_20->addWidget(requestTargetAcquiredpushButton);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        targetAcquiredlabel = new QLabel(layoutWidget1);
        targetAcquiredlabel->setObjectName(QStringLiteral("targetAcquiredlabel"));

        horizontalLayout_15->addWidget(targetAcquiredlabel);

        target_acquired_bool_label = new QLabel(layoutWidget1);
        target_acquired_bool_label->setObjectName(QStringLiteral("target_acquired_bool_label"));

        horizontalLayout_15->addWidget(target_acquired_bool_label);


        verticalLayout_5->addLayout(horizontalLayout_15);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        targetAcquiredReqSentBoollabel = new QLabel(layoutWidget1);
        targetAcquiredReqSentBoollabel->setObjectName(QStringLiteral("targetAcquiredReqSentBoollabel"));

        horizontalLayout_14->addWidget(targetAcquiredReqSentBoollabel);

        target_Acquired_Request_Sent_Bool_label = new QLabel(layoutWidget1);
        target_Acquired_Request_Sent_Bool_label->setObjectName(QStringLiteral("target_Acquired_Request_Sent_Bool_label"));

        horizontalLayout_14->addWidget(target_Acquired_Request_Sent_Bool_label);


        verticalLayout_5->addLayout(horizontalLayout_14);


        horizontalLayout_20->addLayout(verticalLayout_5);


        verticalLayout_7->addLayout(horizontalLayout_20);


        verticalLayout_17->addLayout(verticalLayout_7);


        verticalLayout_18->addLayout(verticalLayout_17);

        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 550, 1131, 41));
        horizontalLayout_22 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        horizontalLayout_22->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        expectedDegreeLabel = new QLabel(layoutWidget2);
        expectedDegreeLabel->setObjectName(QStringLiteral("expectedDegreeLabel"));
        QFont font1;
        font1.setPointSize(11);
        expectedDegreeLabel->setFont(font1);

        horizontalLayout_21->addWidget(expectedDegreeLabel);

        angleLcdNumber = new QLCDNumber(layoutWidget2);
        angleLcdNumber->setObjectName(QStringLiteral("angleLcdNumber"));
        angleLcdNumber->setSmallDecimalPoint(false);

        horizontalLayout_21->addWidget(angleLcdNumber);


        horizontalLayout_22->addLayout(horizontalLayout_21);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        DistanceFromTargetLabel = new QLabel(layoutWidget2);
        DistanceFromTargetLabel->setObjectName(QStringLiteral("DistanceFromTargetLabel"));
        DistanceFromTargetLabel->setFont(font1);

        horizontalLayout_4->addWidget(DistanceFromTargetLabel);

        distanceFromTargetLcdNumber = new QLCDNumber(layoutWidget2);
        distanceFromTargetLcdNumber->setObjectName(QStringLiteral("distanceFromTargetLcdNumber"));

        horizontalLayout_4->addWidget(distanceFromTargetLcdNumber);


        horizontalLayout_22->addLayout(horizontalLayout_4);

        pfWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(pfWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1162, 21));
        menuCalculate_Variance = new QMenu(menuBar);
        menuCalculate_Variance->setObjectName(QStringLiteral("menuCalculate_Variance"));
        pfWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(pfWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        pfWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(pfWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        pfWindow->setStatusBar(statusBar);

        menuBar->addAction(menuCalculate_Variance->menuAction());
        menuCalculate_Variance->addAction(actionCalculate_Variance);
        menuCalculate_Variance->addAction(actionRequests_Responses);
        menuCalculate_Variance->addAction(actionTG_Requests_and_Responses);

        retranslateUi(pfWindow);

        QMetaObject::connectSlotsByName(pfWindow);
    } // setupUi

    void retranslateUi(QMainWindow *pfWindow)
    {
        pfWindow->setWindowTitle(QApplication::translate("pfWindow", "pfWindow", 0));
        actionCalculate_Variance->setText(QApplication::translate("pfWindow", "Calculate Variance", 0));
        actionRequests_Responses->setText(QApplication::translate("pfWindow", "PF Requests and Responses", 0));
        actionTG_Requests_and_Responses->setText(QApplication::translate("pfWindow", "TG Requests and Responses", 0));
        ipAddressLabel->setText(QApplication::translate("pfWindow", "IP Address:", 0));
        serverIPLineEdit->setText(QApplication::translate("pfWindow", "192.168.0.5", 0));
        portLabel->setText(QApplication::translate("pfWindow", "Port: ", 0));
        portLineEdit->setText(QApplication::translate("pfWindow", "2000", 0));
        connectedLabel->setText(QApplication::translate("pfWindow", "Connected:", 0));
        connectionStatusLabel->setText(QApplication::translate("pfWindow", "No", 0));
        connectToServerButton->setText(QApplication::translate("pfWindow", "Connect To Server", 0));
        disconnectFromServerButton->setText(QApplication::translate("pfWindow", "Disconnect From Server", 0));
        startSensingButton->setText(QApplication::translate("pfWindow", "Start Reading Sensors", 0));
        numberOfSensorValuesReceivedLabel->setText(QApplication::translate("pfWindow", "Number of Sensor Values Received", 0));
        IR_0Label->setText(QApplication::translate("pfWindow", "IR_0:", 0));
        IR_0CountLabel->setText(QApplication::translate("pfWindow", "0", 0));
        IR_1Label->setText(QApplication::translate("pfWindow", "IR_1:", 0));
        IR_1CountLabel->setText(QApplication::translate("pfWindow", "0", 0));
        IR_2Label->setText(QApplication::translate("pfWindow", "IR_2:", 0));
        IR_2CountLabel->setText(QApplication::translate("pfWindow", "0", 0));
        IR_3label->setText(QApplication::translate("pfWindow", "IR_3:", 0));
        IR_3CountLabel->setText(QApplication::translate("pfWindow", "0", 0));
        IR_4label->setText(QApplication::translate("pfWindow", "IR_4:", 0));
        IR_4CountLabel->setText(QApplication::translate("pfWindow", "0", 0));
        GrabberIsAlignedLabel->setText(QApplication::translate("pfWindow", "Grabber is Aligned:", 0));
        Grabber_AlignedLabel->setText(QApplication::translate("pfWindow", "No", 0));
        label->setText(QApplication::translate("pfWindow", "IR_0", 0));
        IR_0_boolLabel->setText(QApplication::translate("pfWindow", "IR_0_bool", 0));
        label_2->setText(QApplication::translate("pfWindow", "IR_1", 0));
        IR_1_boolLabel->setText(QApplication::translate("pfWindow", "IR_1_bool", 0));
        label_3->setText(QApplication::translate("pfWindow", "IR_2", 0));
        IR_2_boolLabel->setText(QApplication::translate("pfWindow", "IR_2_bool", 0));
        IR_3collabel->setText(QApplication::translate("pfWindow", "IR_3", 0));
        IR_3label_2->setText(QApplication::translate("pfWindow", "IR_3_bool", 0));
        IR_4collabel->setText(QApplication::translate("pfWindow", "IR_4", 0));
        IR_3label_3->setText(QApplication::translate("pfWindow", "IR_4_bool", 0));
        AlignmentInfoLabel->setText(QApplication::translate("pfWindow", "Alignment Information:", 0));
        Alignment_InfoLabel->setText(QApplication::translate("pfWindow", "N/A", 0));
        proximityPushButton->setText(QApplication::translate("pfWindow", "Send Proximity", 0));
        proxsentboollabel->setText(QApplication::translate("pfWindow", "Proximity Sent Bool:", 0));
        prox_sent_bool_label->setText(QApplication::translate("pfWindow", "N/A ", 0));
        proximityValueLabel->setText(QApplication::translate("pfWindow", "Proximity Value:", 0));
        sendMovementPushButton->setText(QApplication::translate("pfWindow", "Send Movement", 0));
        movementBoollabel->setText(QApplication::translate("pfWindow", "Movement Bool:", 0));
        movement_bool_label->setText(QApplication::translate("pfWindow", "N/A", 0));
        sendOutsideofArenaPushButton->setText(QApplication::translate("pfWindow", "Send Outside of Arena", 0));
        outsideofArenaBoollabel->setText(QApplication::translate("pfWindow", "Outside of Arena Bool:", 0));
        outside_of_arena_bool_label->setText(QApplication::translate("pfWindow", "N/A", 0));
        requestTargetAcquiredpushButton->setText(QApplication::translate("pfWindow", "Request Target Acquired", 0));
        targetAcquiredlabel->setText(QApplication::translate("pfWindow", "Target Acquired:", 0));
        target_acquired_bool_label->setText(QApplication::translate("pfWindow", "N/A", 0));
        targetAcquiredReqSentBoollabel->setText(QApplication::translate("pfWindow", "Target Acquired Request Sent:", 0));
        target_Acquired_Request_Sent_Bool_label->setText(QApplication::translate("pfWindow", "N/A", 0));
        expectedDegreeLabel->setText(QApplication::translate("pfWindow", "Expected Degree(s) to Turn:", 0));
        DistanceFromTargetLabel->setText(QApplication::translate("pfWindow", "Distance from center of Rover to Target:", 0));
        menuCalculate_Variance->setTitle(QApplication::translate("pfWindow", "Additional Features", 0));
    } // retranslateUi

};

namespace Ui {
    class pfWindow: public Ui_pfWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PFWINDOW_H
