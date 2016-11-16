/********************************************************************************
** Form generated from reading UI file 'proximitywindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROXIMITYWINDOW_H
#define UI_PROXIMITYWINDOW_H

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
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_proximityWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget_2;
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
    QWidget *layoutWidget_3;
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
    QWidget *widget;
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
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *proximityWindow)
    {
        if (proximityWindow->objectName().isEmpty())
            proximityWindow->setObjectName(QStringLiteral("proximityWindow"));
        proximityWindow->resize(760, 459);
        centralWidget = new QWidget(proximityWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget_2 = new QWidget(centralWidget);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(11, 133, 681, 181));
        verticalLayout = new QVBoxLayout(layoutWidget_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_10 = new QSpacerItem(6, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        label = new QLabel(layoutWidget_2);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label);

        horizontalSpacer_7 = new QSpacerItem(47, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        IR_0_boolLabel = new QLabel(layoutWidget_2);
        IR_0_boolLabel->setObjectName(QStringLiteral("IR_0_boolLabel"));

        horizontalLayout_4->addWidget(IR_0_boolLabel);

        horizontalSpacer_5 = new QSpacerItem(11, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        label_2 = new QLabel(layoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_2);

        horizontalSpacer_8 = new QSpacerItem(56, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        IR_1_boolLabel = new QLabel(layoutWidget_2);
        IR_1_boolLabel->setObjectName(QStringLiteral("IR_1_boolLabel"));

        horizontalLayout_4->addWidget(IR_1_boolLabel);

        horizontalSpacer_6 = new QSpacerItem(24, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        label_3 = new QLabel(layoutWidget_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_3);

        horizontalSpacer_9 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);

        IR_2_boolLabel = new QLabel(layoutWidget_2);
        IR_2_boolLabel->setObjectName(QStringLiteral("IR_2_boolLabel"));

        horizontalLayout_4->addWidget(IR_2_boolLabel);

        horizontalSpacer_11 = new QSpacerItem(9, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_11);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        IR_0PlainTextEdit = new QPlainTextEdit(layoutWidget_2);
        IR_0PlainTextEdit->setObjectName(QStringLiteral("IR_0PlainTextEdit"));

        horizontalLayout->addWidget(IR_0PlainTextEdit);

        IR_0_bool_textEdit = new QTextEdit(layoutWidget_2);
        IR_0_bool_textEdit->setObjectName(QStringLiteral("IR_0_bool_textEdit"));

        horizontalLayout->addWidget(IR_0_bool_textEdit);

        horizontalSpacer_3 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        IR_1PlainTextEdit = new QPlainTextEdit(layoutWidget_2);
        IR_1PlainTextEdit->setObjectName(QStringLiteral("IR_1PlainTextEdit"));

        horizontalLayout->addWidget(IR_1PlainTextEdit);

        IR_1_bool_textEdit = new QTextEdit(layoutWidget_2);
        IR_1_bool_textEdit->setObjectName(QStringLiteral("IR_1_bool_textEdit"));

        horizontalLayout->addWidget(IR_1_bool_textEdit);

        horizontalSpacer_4 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        IR_2PlainTextEdit = new QPlainTextEdit(layoutWidget_2);
        IR_2PlainTextEdit->setObjectName(QStringLiteral("IR_2PlainTextEdit"));

        horizontalLayout->addWidget(IR_2PlainTextEdit);

        IR_2_bool_textEdit = new QTextEdit(layoutWidget_2);
        IR_2_bool_textEdit->setObjectName(QStringLiteral("IR_2_bool_textEdit"));

        horizontalLayout->addWidget(IR_2_bool_textEdit);


        verticalLayout->addLayout(horizontalLayout);

        layoutWidget_3 = new QWidget(centralWidget);
        layoutWidget_3->setObjectName(QStringLiteral("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(353, 3, 341, 131));
        verticalLayout_2 = new QVBoxLayout(layoutWidget_3);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        startSensingButton = new QPushButton(layoutWidget_3);
        startSensingButton->setObjectName(QStringLiteral("startSensingButton"));

        verticalLayout_2->addWidget(startSensingButton);

        numberOfSensorValuesReceivedLabel = new QLabel(layoutWidget_3);
        numberOfSensorValuesReceivedLabel->setObjectName(QStringLiteral("numberOfSensorValuesReceivedLabel"));
        numberOfSensorValuesReceivedLabel->setAlignment(Qt::AlignCenter);
        numberOfSensorValuesReceivedLabel->setWordWrap(false);

        verticalLayout_2->addWidget(numberOfSensorValuesReceivedLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        IR_0Label = new QLabel(layoutWidget_3);
        IR_0Label->setObjectName(QStringLiteral("IR_0Label"));

        horizontalLayout_3->addWidget(IR_0Label);

        IR_0CountLabel = new QLabel(layoutWidget_3);
        IR_0CountLabel->setObjectName(QStringLiteral("IR_0CountLabel"));

        horizontalLayout_3->addWidget(IR_0CountLabel);

        IR_1Label = new QLabel(layoutWidget_3);
        IR_1Label->setObjectName(QStringLiteral("IR_1Label"));

        horizontalLayout_3->addWidget(IR_1Label);

        IR_1CountLabel = new QLabel(layoutWidget_3);
        IR_1CountLabel->setObjectName(QStringLiteral("IR_1CountLabel"));

        horizontalLayout_3->addWidget(IR_1CountLabel);

        IR_2Label = new QLabel(layoutWidget_3);
        IR_2Label->setObjectName(QStringLiteral("IR_2Label"));

        horizontalLayout_3->addWidget(IR_2Label);

        IR_2CountLabel = new QLabel(layoutWidget_3);
        IR_2CountLabel->setObjectName(QStringLiteral("IR_2CountLabel"));

        horizontalLayout_3->addWidget(IR_2CountLabel);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        GrabberIsAlignedLabel = new QLabel(layoutWidget_3);
        GrabberIsAlignedLabel->setObjectName(QStringLiteral("GrabberIsAlignedLabel"));

        horizontalLayout_2->addWidget(GrabberIsAlignedLabel);

        Grabber_AlignedLabel = new QLabel(layoutWidget_3);
        Grabber_AlignedLabel->setObjectName(QStringLiteral("Grabber_AlignedLabel"));

        horizontalLayout_2->addWidget(Grabber_AlignedLabel);


        verticalLayout_2->addLayout(horizontalLayout_2);

        layoutWidget_4 = new QWidget(centralWidget);
        layoutWidget_4->setObjectName(QStringLiteral("layoutWidget_4"));
        layoutWidget_4->setGeometry(QRect(10, 0, 339, 134));
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

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(17, 320, 671, 51));
        horizontalLayout_13 = new QHBoxLayout(widget);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        AlignmentInfoLabel = new QLabel(widget);
        AlignmentInfoLabel->setObjectName(QStringLiteral("AlignmentInfoLabel"));

        horizontalLayout_5->addWidget(AlignmentInfoLabel);

        Alignment_InfoLabel = new QLabel(widget);
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
        proximityPushButton = new QPushButton(widget);
        proximityPushButton->setObjectName(QStringLiteral("proximityPushButton"));

        horizontalLayout_12->addWidget(proximityPushButton);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        proxsentboollabel = new QLabel(widget);
        proxsentboollabel->setObjectName(QStringLiteral("proxsentboollabel"));

        horizontalLayout_10->addWidget(proxsentboollabel);

        prox_sent_bool_label = new QLabel(widget);
        prox_sent_bool_label->setObjectName(QStringLiteral("prox_sent_bool_label"));

        horizontalLayout_10->addWidget(prox_sent_bool_label);


        horizontalLayout_12->addLayout(horizontalLayout_10);


        verticalLayout_3->addLayout(horizontalLayout_12);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        proximityValueLabel = new QLabel(widget);
        proximityValueLabel->setObjectName(QStringLiteral("proximityValueLabel"));

        horizontalLayout_11->addWidget(proximityValueLabel);

        proximitySlider = new QSlider(widget);
        proximitySlider->setObjectName(QStringLiteral("proximitySlider"));
        proximitySlider->setMaximum(20);
        proximitySlider->setValue(10);
        proximitySlider->setSliderPosition(10);
        proximitySlider->setOrientation(Qt::Horizontal);

        horizontalLayout_11->addWidget(proximitySlider);

        proxlcdNumber = new QLCDNumber(widget);
        proxlcdNumber->setObjectName(QStringLiteral("proxlcdNumber"));
        proxlcdNumber->setProperty("intValue", QVariant(10));

        horizontalLayout_11->addWidget(proxlcdNumber);


        verticalLayout_3->addLayout(horizontalLayout_11);


        horizontalLayout_13->addLayout(verticalLayout_3);

        proximityWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(proximityWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 760, 17));
        proximityWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(proximityWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        proximityWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(proximityWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        proximityWindow->setStatusBar(statusBar);

        retranslateUi(proximityWindow);

        QMetaObject::connectSlotsByName(proximityWindow);
    } // setupUi

    void retranslateUi(QMainWindow *proximityWindow)
    {
        proximityWindow->setWindowTitle(QApplication::translate("proximityWindow", "proximityWindow", 0));
        label->setText(QApplication::translate("proximityWindow", "IR_0", 0));
        IR_0_boolLabel->setText(QApplication::translate("proximityWindow", "IR_0_bool", 0));
        label_2->setText(QApplication::translate("proximityWindow", "IR_1", 0));
        IR_1_boolLabel->setText(QApplication::translate("proximityWindow", "IR_1_bool", 0));
        label_3->setText(QApplication::translate("proximityWindow", "IR_2", 0));
        IR_2_boolLabel->setText(QApplication::translate("proximityWindow", "IR_2_bool", 0));
        startSensingButton->setText(QApplication::translate("proximityWindow", "Start Reading Sensors", 0));
        numberOfSensorValuesReceivedLabel->setText(QApplication::translate("proximityWindow", "Number of Sensor Values Received", 0));
        IR_0Label->setText(QApplication::translate("proximityWindow", "IR_0:", 0));
        IR_0CountLabel->setText(QApplication::translate("proximityWindow", "0", 0));
        IR_1Label->setText(QApplication::translate("proximityWindow", "IR_1:", 0));
        IR_1CountLabel->setText(QApplication::translate("proximityWindow", "0", 0));
        IR_2Label->setText(QApplication::translate("proximityWindow", "IR_2:", 0));
        IR_2CountLabel->setText(QApplication::translate("proximityWindow", "0", 0));
        GrabberIsAlignedLabel->setText(QApplication::translate("proximityWindow", "Grabber is Aligned:", 0));
        Grabber_AlignedLabel->setText(QApplication::translate("proximityWindow", "No", 0));
        ipAddressLabel->setText(QApplication::translate("proximityWindow", "IP Address:", 0));
        serverIPLineEdit->setText(QApplication::translate("proximityWindow", "192.168.0.197", 0));
        portLabel->setText(QApplication::translate("proximityWindow", "Port: ", 0));
        portLineEdit->setText(QApplication::translate("proximityWindow", "2000", 0));
        connectedLabel->setText(QApplication::translate("proximityWindow", "Connected:", 0));
        connectionStatusLabel->setText(QApplication::translate("proximityWindow", "No", 0));
        connectToServerButton->setText(QApplication::translate("proximityWindow", "Connect To Server", 0));
        disconnectFromServerButton->setText(QApplication::translate("proximityWindow", "Disconnect From Server", 0));
        AlignmentInfoLabel->setText(QApplication::translate("proximityWindow", "Alignment Information:", 0));
        Alignment_InfoLabel->setText(QApplication::translate("proximityWindow", "N/A", 0));
        proximityPushButton->setText(QApplication::translate("proximityWindow", "Send Proximity", 0));
        proxsentboollabel->setText(QApplication::translate("proximityWindow", "Proximity Sent Bool:", 0));
        prox_sent_bool_label->setText(QApplication::translate("proximityWindow", "N/A ", 0));
        proximityValueLabel->setText(QApplication::translate("proximityWindow", "Proximity Value:", 0));
    } // retranslateUi

};

namespace Ui {
    class proximityWindow: public Ui_proximityWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROXIMITYWINDOW_H
