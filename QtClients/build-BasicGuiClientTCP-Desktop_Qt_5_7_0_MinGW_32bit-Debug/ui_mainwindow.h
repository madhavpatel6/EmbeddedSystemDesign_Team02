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
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout;
    QLabel *label_23;
    QHBoxLayout *HL_Label;
    QLabel *GoodMsgLabel;
    QLabel *CommErrorLabel;
    QLabel *label_3;
    QLabel *ReqRecvLabel;
    QLabel *ResRecvLabel;
    QLabel *ReqSentLabel;
    QLabel *ResSentReceived;
    QLabel *labelSM;
    QHBoxLayout *HL_SM;
    QLabel *SMGoodMsg;
    QLabel *SMCommError;
    QLabel *SMPercError;
    QLabel *SMReqRecv;
    QLabel *SMResRecv;
    QLabel *SMReqSent;
    QLabel *SMResSent;
    QLabel *labelTL;
    QHBoxLayout *HL_TL;
    QLabel *TLGoodMsg;
    QLabel *TLCommError;
    QLabel *TLPercError;
    QLabel *TLReqRecv;
    QLabel *TLResRecv;
    QLabel *TLReqSent;
    QLabel *TLResSent;
    QLabel *labelPF;
    QHBoxLayout *HL_PF;
    QLabel *PFGoodMsg;
    QLabel *PFCommError;
    QLabel *PFPercError;
    QLabel *PFReqRecv;
    QLabel *PFResRecv;
    QLabel *PFReqSent;
    QLabel *PFResSent;
    QLabel *labelTG;
    QHBoxLayout *HL_TG;
    QLabel *TGGoodMsg;
    QLabel *TGCommError;
    QLabel *TGPercError;
    QLabel *TGReqRecv;
    QLabel *TGResRecv;
    QLabel *TGReqSent;
    QLabel *TGResSent;
    QLabel *RequestSentLabel;
    QLabel *RequestSentCount;
    QLabel *serverErrorLabel;
    QLabel *serverErrorVal;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QLineEdit *serverIPAddressInput;
    QLabel *label_2;
    QLineEdit *serverPortInput;
    QPushButton *connectToServer;
    QLabel *sentCommStatLabel;
    QPushButton *commStatsButton;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(579, 358);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_23 = new QLabel(centralWidget);
        label_23->setObjectName(QStringLiteral("label_23"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_23);

        HL_Label = new QHBoxLayout();
        HL_Label->setSpacing(6);
        HL_Label->setObjectName(QStringLiteral("HL_Label"));
        GoodMsgLabel = new QLabel(centralWidget);
        GoodMsgLabel->setObjectName(QStringLiteral("GoodMsgLabel"));
        GoodMsgLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_Label->addWidget(GoodMsgLabel);

        CommErrorLabel = new QLabel(centralWidget);
        CommErrorLabel->setObjectName(QStringLiteral("CommErrorLabel"));
        CommErrorLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_Label->addWidget(CommErrorLabel);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_Label->addWidget(label_3);

        ReqRecvLabel = new QLabel(centralWidget);
        ReqRecvLabel->setObjectName(QStringLiteral("ReqRecvLabel"));
        ReqRecvLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_Label->addWidget(ReqRecvLabel);

        ResRecvLabel = new QLabel(centralWidget);
        ResRecvLabel->setObjectName(QStringLiteral("ResRecvLabel"));
        ResRecvLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_Label->addWidget(ResRecvLabel);

        ReqSentLabel = new QLabel(centralWidget);
        ReqSentLabel->setObjectName(QStringLiteral("ReqSentLabel"));
        ReqSentLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_Label->addWidget(ReqSentLabel);

        ResSentReceived = new QLabel(centralWidget);
        ResSentReceived->setObjectName(QStringLiteral("ResSentReceived"));
        ResSentReceived->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_Label->addWidget(ResSentReceived);


        formLayout->setLayout(0, QFormLayout::FieldRole, HL_Label);

        labelSM = new QLabel(centralWidget);
        labelSM->setObjectName(QStringLiteral("labelSM"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelSM);

        HL_SM = new QHBoxLayout();
        HL_SM->setSpacing(6);
        HL_SM->setObjectName(QStringLiteral("HL_SM"));
        SMGoodMsg = new QLabel(centralWidget);
        SMGoodMsg->setObjectName(QStringLiteral("SMGoodMsg"));
        SMGoodMsg->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_SM->addWidget(SMGoodMsg);

        SMCommError = new QLabel(centralWidget);
        SMCommError->setObjectName(QStringLiteral("SMCommError"));
        SMCommError->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_SM->addWidget(SMCommError);

        SMPercError = new QLabel(centralWidget);
        SMPercError->setObjectName(QStringLiteral("SMPercError"));
        SMPercError->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_SM->addWidget(SMPercError);

        SMReqRecv = new QLabel(centralWidget);
        SMReqRecv->setObjectName(QStringLiteral("SMReqRecv"));
        SMReqRecv->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_SM->addWidget(SMReqRecv);

        SMResRecv = new QLabel(centralWidget);
        SMResRecv->setObjectName(QStringLiteral("SMResRecv"));
        SMResRecv->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_SM->addWidget(SMResRecv);

        SMReqSent = new QLabel(centralWidget);
        SMReqSent->setObjectName(QStringLiteral("SMReqSent"));
        SMReqSent->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_SM->addWidget(SMReqSent);

        SMResSent = new QLabel(centralWidget);
        SMResSent->setObjectName(QStringLiteral("SMResSent"));
        SMResSent->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_SM->addWidget(SMResSent);


        formLayout->setLayout(1, QFormLayout::FieldRole, HL_SM);

        labelTL = new QLabel(centralWidget);
        labelTL->setObjectName(QStringLiteral("labelTL"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelTL);

        HL_TL = new QHBoxLayout();
        HL_TL->setSpacing(6);
        HL_TL->setObjectName(QStringLiteral("HL_TL"));
        TLGoodMsg = new QLabel(centralWidget);
        TLGoodMsg->setObjectName(QStringLiteral("TLGoodMsg"));
        TLGoodMsg->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TL->addWidget(TLGoodMsg);

        TLCommError = new QLabel(centralWidget);
        TLCommError->setObjectName(QStringLiteral("TLCommError"));
        TLCommError->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TL->addWidget(TLCommError);

        TLPercError = new QLabel(centralWidget);
        TLPercError->setObjectName(QStringLiteral("TLPercError"));
        TLPercError->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TL->addWidget(TLPercError);

        TLReqRecv = new QLabel(centralWidget);
        TLReqRecv->setObjectName(QStringLiteral("TLReqRecv"));
        TLReqRecv->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TL->addWidget(TLReqRecv);

        TLResRecv = new QLabel(centralWidget);
        TLResRecv->setObjectName(QStringLiteral("TLResRecv"));
        TLResRecv->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TL->addWidget(TLResRecv);

        TLReqSent = new QLabel(centralWidget);
        TLReqSent->setObjectName(QStringLiteral("TLReqSent"));
        TLReqSent->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TL->addWidget(TLReqSent);

        TLResSent = new QLabel(centralWidget);
        TLResSent->setObjectName(QStringLiteral("TLResSent"));
        TLResSent->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TL->addWidget(TLResSent);


        formLayout->setLayout(2, QFormLayout::FieldRole, HL_TL);

        labelPF = new QLabel(centralWidget);
        labelPF->setObjectName(QStringLiteral("labelPF"));

        formLayout->setWidget(3, QFormLayout::LabelRole, labelPF);

        HL_PF = new QHBoxLayout();
        HL_PF->setSpacing(6);
        HL_PF->setObjectName(QStringLiteral("HL_PF"));
        PFGoodMsg = new QLabel(centralWidget);
        PFGoodMsg->setObjectName(QStringLiteral("PFGoodMsg"));
        PFGoodMsg->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_PF->addWidget(PFGoodMsg);

        PFCommError = new QLabel(centralWidget);
        PFCommError->setObjectName(QStringLiteral("PFCommError"));
        PFCommError->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_PF->addWidget(PFCommError);

        PFPercError = new QLabel(centralWidget);
        PFPercError->setObjectName(QStringLiteral("PFPercError"));
        PFPercError->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_PF->addWidget(PFPercError);

        PFReqRecv = new QLabel(centralWidget);
        PFReqRecv->setObjectName(QStringLiteral("PFReqRecv"));
        PFReqRecv->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_PF->addWidget(PFReqRecv);

        PFResRecv = new QLabel(centralWidget);
        PFResRecv->setObjectName(QStringLiteral("PFResRecv"));
        PFResRecv->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_PF->addWidget(PFResRecv);

        PFReqSent = new QLabel(centralWidget);
        PFReqSent->setObjectName(QStringLiteral("PFReqSent"));
        PFReqSent->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_PF->addWidget(PFReqSent);

        PFResSent = new QLabel(centralWidget);
        PFResSent->setObjectName(QStringLiteral("PFResSent"));
        PFResSent->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_PF->addWidget(PFResSent);


        formLayout->setLayout(3, QFormLayout::FieldRole, HL_PF);

        labelTG = new QLabel(centralWidget);
        labelTG->setObjectName(QStringLiteral("labelTG"));

        formLayout->setWidget(4, QFormLayout::LabelRole, labelTG);

        HL_TG = new QHBoxLayout();
        HL_TG->setSpacing(6);
        HL_TG->setObjectName(QStringLiteral("HL_TG"));
        TGGoodMsg = new QLabel(centralWidget);
        TGGoodMsg->setObjectName(QStringLiteral("TGGoodMsg"));
        TGGoodMsg->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TG->addWidget(TGGoodMsg);

        TGCommError = new QLabel(centralWidget);
        TGCommError->setObjectName(QStringLiteral("TGCommError"));
        TGCommError->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TG->addWidget(TGCommError);

        TGPercError = new QLabel(centralWidget);
        TGPercError->setObjectName(QStringLiteral("TGPercError"));
        TGPercError->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TG->addWidget(TGPercError);

        TGReqRecv = new QLabel(centralWidget);
        TGReqRecv->setObjectName(QStringLiteral("TGReqRecv"));
        TGReqRecv->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TG->addWidget(TGReqRecv);

        TGResRecv = new QLabel(centralWidget);
        TGResRecv->setObjectName(QStringLiteral("TGResRecv"));
        TGResRecv->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TG->addWidget(TGResRecv);

        TGReqSent = new QLabel(centralWidget);
        TGReqSent->setObjectName(QStringLiteral("TGReqSent"));
        TGReqSent->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TG->addWidget(TGReqSent);

        TGResSent = new QLabel(centralWidget);
        TGResSent->setObjectName(QStringLiteral("TGResSent"));
        TGResSent->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        HL_TG->addWidget(TGResSent);


        formLayout->setLayout(4, QFormLayout::FieldRole, HL_TG);

        RequestSentLabel = new QLabel(centralWidget);
        RequestSentLabel->setObjectName(QStringLiteral("RequestSentLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, RequestSentLabel);

        RequestSentCount = new QLabel(centralWidget);
        RequestSentCount->setObjectName(QStringLiteral("RequestSentCount"));
        RequestSentCount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(5, QFormLayout::FieldRole, RequestSentCount);

        serverErrorLabel = new QLabel(centralWidget);
        serverErrorLabel->setObjectName(QStringLiteral("serverErrorLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, serverErrorLabel);

        serverErrorVal = new QLabel(centralWidget);
        serverErrorVal->setObjectName(QStringLiteral("serverErrorVal"));
        serverErrorVal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(6, QFormLayout::FieldRole, serverErrorVal);


        horizontalLayout->addLayout(formLayout);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_3->addWidget(label);

        serverIPAddressInput = new QLineEdit(centralWidget);
        serverIPAddressInput->setObjectName(QStringLiteral("serverIPAddressInput"));

        verticalLayout_3->addWidget(serverIPAddressInput);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_3->addWidget(label_2);

        serverPortInput = new QLineEdit(centralWidget);
        serverPortInput->setObjectName(QStringLiteral("serverPortInput"));

        verticalLayout_3->addWidget(serverPortInput);

        connectToServer = new QPushButton(centralWidget);
        connectToServer->setObjectName(QStringLiteral("connectToServer"));

        verticalLayout_3->addWidget(connectToServer);

        sentCommStatLabel = new QLabel(centralWidget);
        sentCommStatLabel->setObjectName(QStringLiteral("sentCommStatLabel"));

        verticalLayout_3->addWidget(sentCommStatLabel);

        commStatsButton = new QPushButton(centralWidget);
        commStatsButton->setObjectName(QStringLiteral("commStatsButton"));

        verticalLayout_3->addWidget(commStatsButton);


        verticalLayout_2->addLayout(verticalLayout_3);

        MainWindow->setCentralWidget(centralWidget);
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Comm Stats GUI", 0));
        label_23->setText(QString());
        GoodMsgLabel->setText(QApplication::translate("MainWindow", "Good Msg", 0));
        CommErrorLabel->setText(QApplication::translate("MainWindow", "Comm Errors", 0));
        label_3->setText(QApplication::translate("MainWindow", "% Error", 0));
        ReqRecvLabel->setText(QApplication::translate("MainWindow", "Req Received", 0));
        ResRecvLabel->setText(QApplication::translate("MainWindow", "Res Received", 0));
        ReqSentLabel->setText(QApplication::translate("MainWindow", "Req Sent", 0));
        ResSentReceived->setText(QApplication::translate("MainWindow", "Res Sent", 0));
        labelSM->setText(QApplication::translate("MainWindow", "Searcher Mover", 0));
        SMGoodMsg->setText(QApplication::translate("MainWindow", "0", 0));
        SMCommError->setText(QApplication::translate("MainWindow", "0", 0));
        SMPercError->setText(QApplication::translate("MainWindow", "0", 0));
        SMReqRecv->setText(QApplication::translate("MainWindow", "0", 0));
        SMResRecv->setText(QApplication::translate("MainWindow", "0", 0));
        SMReqSent->setText(QApplication::translate("MainWindow", "0", 0));
        SMResSent->setText(QApplication::translate("MainWindow", "0", 0));
        labelTL->setText(QApplication::translate("MainWindow", "Target Locator", 0));
        TLGoodMsg->setText(QApplication::translate("MainWindow", "0", 0));
        TLCommError->setText(QApplication::translate("MainWindow", "0", 0));
        TLPercError->setText(QApplication::translate("MainWindow", "0", 0));
        TLReqRecv->setText(QApplication::translate("MainWindow", "0", 0));
        TLResRecv->setText(QApplication::translate("MainWindow", "0", 0));
        TLReqSent->setText(QApplication::translate("MainWindow", "0", 0));
        TLResSent->setText(QApplication::translate("MainWindow", "0", 0));
        labelPF->setText(QApplication::translate("MainWindow", "Path Finder", 0));
        PFGoodMsg->setText(QApplication::translate("MainWindow", "0", 0));
        PFCommError->setText(QApplication::translate("MainWindow", "0", 0));
        PFPercError->setText(QApplication::translate("MainWindow", "0", 0));
        PFReqRecv->setText(QApplication::translate("MainWindow", "0", 0));
        PFResRecv->setText(QApplication::translate("MainWindow", "0", 0));
        PFReqSent->setText(QApplication::translate("MainWindow", "0", 0));
        PFResSent->setText(QApplication::translate("MainWindow", "0", 0));
        labelTG->setText(QApplication::translate("MainWindow", "Target Grabber", 0));
        TGGoodMsg->setText(QApplication::translate("MainWindow", "0", 0));
        TGCommError->setText(QApplication::translate("MainWindow", "0", 0));
        TGPercError->setText(QApplication::translate("MainWindow", "0", 0));
        TGReqRecv->setText(QApplication::translate("MainWindow", "0", 0));
        TGResRecv->setText(QApplication::translate("MainWindow", "0", 0));
        TGReqSent->setText(QApplication::translate("MainWindow", "0", 0));
        TGResSent->setText(QApplication::translate("MainWindow", "0", 0));
        RequestSentLabel->setText(QApplication::translate("MainWindow", "Request Sent", 0));
        RequestSentCount->setText(QApplication::translate("MainWindow", "0", 0));
        serverErrorLabel->setText(QApplication::translate("MainWindow", "Server Errors", 0));
        serverErrorVal->setText(QApplication::translate("MainWindow", "0", 0));
        label->setText(QApplication::translate("MainWindow", "Server IP Address", 0));
        serverIPAddressInput->setText(QApplication::translate("MainWindow", "192.168.0.197", 0));
        label_2->setText(QApplication::translate("MainWindow", "Server Port", 0));
        serverPortInput->setText(QApplication::translate("MainWindow", "2000", 0));
        connectToServer->setText(QApplication::translate("MainWindow", "Connect to Server", 0));
        sentCommStatLabel->setText(QApplication::translate("MainWindow", "Sent commStat:", 0));
        commStatsButton->setText(QApplication::translate("MainWindow", "Request Comm Stats", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
