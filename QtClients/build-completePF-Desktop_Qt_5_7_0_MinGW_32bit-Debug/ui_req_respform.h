/********************************************************************************
** Form generated from reading UI file 'req_respform.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REQ_RESPFORM_H
#define UI_REQ_RESPFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_req_respForm
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *curReqlabel;
    QSpacerItem *horizontalSpacer;
    QPlainTextEdit *curReqPlainTextEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *curResplabel;
    QSpacerItem *horizontalSpacer_2;
    QPlainTextEdit *curRespPlainTextEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *prevReqlabel;
    QSpacerItem *horizontalSpacer_3;
    QPlainTextEdit *prevReqPlainTextEdit;
    QHBoxLayout *horizontalLayout;
    QLabel *prevResplabel;
    QSpacerItem *horizontalSpacer_4;
    QPlainTextEdit *prevRespPlainTextEdit;
    QLabel *label;

    void setupUi(QWidget *req_respForm)
    {
        if (req_respForm->objectName().isEmpty())
            req_respForm->setObjectName(QStringLiteral("req_respForm"));
        req_respForm->resize(657, 391);
        layoutWidget = new QWidget(req_respForm);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 60, 631, 312));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        curReqlabel = new QLabel(layoutWidget);
        curReqlabel->setObjectName(QStringLiteral("curReqlabel"));

        horizontalLayout_4->addWidget(curReqlabel);

        horizontalSpacer = new QSpacerItem(44, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        curReqPlainTextEdit = new QPlainTextEdit(layoutWidget);
        curReqPlainTextEdit->setObjectName(QStringLiteral("curReqPlainTextEdit"));

        horizontalLayout_4->addWidget(curReqPlainTextEdit);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        curResplabel = new QLabel(layoutWidget);
        curResplabel->setObjectName(QStringLiteral("curResplabel"));

        horizontalLayout_3->addWidget(curResplabel);

        horizontalSpacer_2 = new QSpacerItem(37, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        curRespPlainTextEdit = new QPlainTextEdit(layoutWidget);
        curRespPlainTextEdit->setObjectName(QStringLiteral("curRespPlainTextEdit"));

        horizontalLayout_3->addWidget(curRespPlainTextEdit);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        prevReqlabel = new QLabel(layoutWidget);
        prevReqlabel->setObjectName(QStringLiteral("prevReqlabel"));

        horizontalLayout_2->addWidget(prevReqlabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        prevReqPlainTextEdit = new QPlainTextEdit(layoutWidget);
        prevReqPlainTextEdit->setObjectName(QStringLiteral("prevReqPlainTextEdit"));

        horizontalLayout_2->addWidget(prevReqPlainTextEdit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        prevResplabel = new QLabel(layoutWidget);
        prevResplabel->setObjectName(QStringLiteral("prevResplabel"));

        horizontalLayout->addWidget(prevResplabel);

        horizontalSpacer_4 = new QSpacerItem(33, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        prevRespPlainTextEdit = new QPlainTextEdit(layoutWidget);
        prevRespPlainTextEdit->setObjectName(QStringLiteral("prevRespPlainTextEdit"));

        horizontalLayout->addWidget(prevRespPlainTextEdit);


        verticalLayout->addLayout(horizontalLayout);

        label = new QLabel(req_respForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(160, 10, 301, 31));
        QFont font;
        font.setPointSize(14);
        label->setFont(font);

        retranslateUi(req_respForm);

        QMetaObject::connectSlotsByName(req_respForm);
    } // setupUi

    void retranslateUi(QWidget *req_respForm)
    {
        req_respForm->setWindowTitle(QApplication::translate("req_respForm", "Form", 0));
        curReqlabel->setText(QApplication::translate("req_respForm", "Current Request:", 0));
        curResplabel->setText(QApplication::translate("req_respForm", "Current Response:", 0));
        prevReqlabel->setText(QApplication::translate("req_respForm", "Previous Request:", 0));
        prevResplabel->setText(QApplication::translate("req_respForm", "Previous Response:", 0));
        label->setText(QApplication::translate("req_respForm", "PathFinder Requests & Responses", 0));
    } // retranslateUi

};

namespace Ui {
    class req_respForm: public Ui_req_respForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REQ_RESPFORM_H
