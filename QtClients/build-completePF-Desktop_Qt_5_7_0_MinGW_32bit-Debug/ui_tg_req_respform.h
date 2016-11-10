/********************************************************************************
** Form generated from reading UI file 'tg_req_respform.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TG_REQ_RESPFORM_H
#define UI_TG_REQ_RESPFORM_H

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

class Ui_tg_req_respForm
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *curReqlabel;
    QSpacerItem *horizontalSpacer;
    QPlainTextEdit *tgcurReqPlainTextEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *curResplabel;
    QSpacerItem *horizontalSpacer_2;
    QPlainTextEdit *tgcurRespPlainTextEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *prevReqlabel;
    QSpacerItem *horizontalSpacer_3;
    QPlainTextEdit *tgprevReqPlainTextEdit;
    QHBoxLayout *horizontalLayout;
    QLabel *prevResplabel;
    QSpacerItem *horizontalSpacer_4;
    QPlainTextEdit *tgprevRespPlainTextEdit;
    QLabel *Titlelabel;

    void setupUi(QWidget *tg_req_respForm)
    {
        if (tg_req_respForm->objectName().isEmpty())
            tg_req_respForm->setObjectName(QStringLiteral("tg_req_respForm"));
        tg_req_respForm->resize(651, 712);
        layoutWidget = new QWidget(tg_req_respForm);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 50, 631, 651));
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

        tgcurReqPlainTextEdit = new QPlainTextEdit(layoutWidget);
        tgcurReqPlainTextEdit->setObjectName(QStringLiteral("tgcurReqPlainTextEdit"));

        horizontalLayout_4->addWidget(tgcurReqPlainTextEdit);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        curResplabel = new QLabel(layoutWidget);
        curResplabel->setObjectName(QStringLiteral("curResplabel"));

        horizontalLayout_3->addWidget(curResplabel);

        horizontalSpacer_2 = new QSpacerItem(37, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        tgcurRespPlainTextEdit = new QPlainTextEdit(layoutWidget);
        tgcurRespPlainTextEdit->setObjectName(QStringLiteral("tgcurRespPlainTextEdit"));

        horizontalLayout_3->addWidget(tgcurRespPlainTextEdit);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        prevReqlabel = new QLabel(layoutWidget);
        prevReqlabel->setObjectName(QStringLiteral("prevReqlabel"));

        horizontalLayout_2->addWidget(prevReqlabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        tgprevReqPlainTextEdit = new QPlainTextEdit(layoutWidget);
        tgprevReqPlainTextEdit->setObjectName(QStringLiteral("tgprevReqPlainTextEdit"));

        horizontalLayout_2->addWidget(tgprevReqPlainTextEdit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        prevResplabel = new QLabel(layoutWidget);
        prevResplabel->setObjectName(QStringLiteral("prevResplabel"));

        horizontalLayout->addWidget(prevResplabel);

        horizontalSpacer_4 = new QSpacerItem(33, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        tgprevRespPlainTextEdit = new QPlainTextEdit(layoutWidget);
        tgprevRespPlainTextEdit->setObjectName(QStringLiteral("tgprevRespPlainTextEdit"));

        horizontalLayout->addWidget(tgprevRespPlainTextEdit);


        verticalLayout->addLayout(horizontalLayout);

        Titlelabel = new QLabel(tg_req_respForm);
        Titlelabel->setObjectName(QStringLiteral("Titlelabel"));
        Titlelabel->setGeometry(QRect(160, 10, 341, 31));
        QFont font;
        font.setPointSize(14);
        Titlelabel->setFont(font);

        retranslateUi(tg_req_respForm);

        QMetaObject::connectSlotsByName(tg_req_respForm);
    } // setupUi

    void retranslateUi(QWidget *tg_req_respForm)
    {
        tg_req_respForm->setWindowTitle(QApplication::translate("tg_req_respForm", "Form", 0));
        curReqlabel->setText(QApplication::translate("tg_req_respForm", "Current Request:", 0));
        curResplabel->setText(QApplication::translate("tg_req_respForm", "Current Response:", 0));
        prevReqlabel->setText(QApplication::translate("tg_req_respForm", "Previous Request:", 0));
        prevResplabel->setText(QApplication::translate("tg_req_respForm", "Previous Response:", 0));
        Titlelabel->setText(QApplication::translate("tg_req_respForm", "Target Grabber Requests & Responses", 0));
    } // retranslateUi

};

namespace Ui {
    class tg_req_respForm: public Ui_tg_req_respForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TG_REQ_RESPFORM_H
