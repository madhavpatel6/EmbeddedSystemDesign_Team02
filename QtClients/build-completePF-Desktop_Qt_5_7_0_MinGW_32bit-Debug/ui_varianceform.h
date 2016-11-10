/********************************************************************************
** Form generated from reading UI file 'varianceform.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VARIANCEFORM_H
#define UI_VARIANCEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_varianceForm
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_7;
    QPushButton *calculateVarianceButton;
    QHBoxLayout *horizontalLayout_5;
    QLabel *varianceStartedLabel;
    QLabel *variance_started_label;
    QVBoxLayout *verticalLayout_6;
    QLabel *varianceIR0Label;
    QLCDNumber *varianceIR0LcdNumber;
    QVBoxLayout *verticalLayout_5;
    QLabel *varianceIR1Label;
    QLCDNumber *varianceIR1LcdNumber;
    QVBoxLayout *verticalLayout_4;
    QLabel *varianceIR2Label;
    QLCDNumber *varianceIR2LcdNumber;
    QVBoxLayout *verticalLayout_3;
    QLabel *varianceIR3Label;
    QLCDNumber *varianceIR3LcdNumber;
    QVBoxLayout *verticalLayout_2;
    QLabel *varianceIR4Label;
    QLCDNumber *varianceIR4LcdNumber;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLCDNumber *meanIR0LcdNumber;
    QLCDNumber *meanIR1LcdNumber;
    QLCDNumber *meanIR2LcdNumber;
    QLCDNumber *meanIR3LcdNumber;
    QLCDNumber *meanIR4LcdNumber;
    QHBoxLayout *horizontalLayout_2;
    QLabel *xiLabel;
    QLCDNumber *ximeanIR0LcdNumber;
    QLCDNumber *ximeanIR1LcdNumber;
    QLCDNumber *ximeanIR2LcdNumber;
    QLCDNumber *ximeanIR3LcdNumber;
    QLCDNumber *ximeanIR4LcdNumber;
    QHBoxLayout *horizontalLayout_3;
    QLabel *ximeansqLabel;
    QLCDNumber *ximeansqIR0LcdNumber;
    QLCDNumber *ximeansqIR1LcdNumber;
    QLCDNumber *ximeansqIR2LcdNumber;
    QLCDNumber *ximeansqIR3LcdNumber;
    QLCDNumber *ximeansqIR4LcdNumber;
    QHBoxLayout *horizontalLayout_4;
    QLabel *sumximeansqLabel;
    QLCDNumber *sumximeansqIR0LcdNumber;
    QLCDNumber *sumximeansqIR1LcdNumber;
    QLCDNumber *sumximeansqIR2LcdNumber;
    QLCDNumber *sumximeansqIR3LcdNumber;
    QLCDNumber *sumximeansqIR4LcdNumber;

    void setupUi(QWidget *varianceForm)
    {
        if (varianceForm->objectName().isEmpty())
            varianceForm->setObjectName(QStringLiteral("varianceForm"));
        varianceForm->resize(1083, 241);
        widget = new QWidget(varianceForm);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(2, 10, 1071, 221));
        verticalLayout_8 = new QVBoxLayout(widget);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        calculateVarianceButton = new QPushButton(widget);
        calculateVarianceButton->setObjectName(QStringLiteral("calculateVarianceButton"));

        verticalLayout_7->addWidget(calculateVarianceButton);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        varianceStartedLabel = new QLabel(widget);
        varianceStartedLabel->setObjectName(QStringLiteral("varianceStartedLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(varianceStartedLabel->sizePolicy().hasHeightForWidth());
        varianceStartedLabel->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(varianceStartedLabel);

        variance_started_label = new QLabel(widget);
        variance_started_label->setObjectName(QStringLiteral("variance_started_label"));

        horizontalLayout_5->addWidget(variance_started_label);


        verticalLayout_7->addLayout(horizontalLayout_5);


        horizontalLayout_6->addLayout(verticalLayout_7);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        varianceIR0Label = new QLabel(widget);
        varianceIR0Label->setObjectName(QStringLiteral("varianceIR0Label"));
        varianceIR0Label->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(varianceIR0Label);

        varianceIR0LcdNumber = new QLCDNumber(widget);
        varianceIR0LcdNumber->setObjectName(QStringLiteral("varianceIR0LcdNumber"));

        verticalLayout_6->addWidget(varianceIR0LcdNumber);


        horizontalLayout_6->addLayout(verticalLayout_6);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        varianceIR1Label = new QLabel(widget);
        varianceIR1Label->setObjectName(QStringLiteral("varianceIR1Label"));
        varianceIR1Label->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(varianceIR1Label);

        varianceIR1LcdNumber = new QLCDNumber(widget);
        varianceIR1LcdNumber->setObjectName(QStringLiteral("varianceIR1LcdNumber"));

        verticalLayout_5->addWidget(varianceIR1LcdNumber);


        horizontalLayout_6->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        varianceIR2Label = new QLabel(widget);
        varianceIR2Label->setObjectName(QStringLiteral("varianceIR2Label"));
        varianceIR2Label->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(varianceIR2Label);

        varianceIR2LcdNumber = new QLCDNumber(widget);
        varianceIR2LcdNumber->setObjectName(QStringLiteral("varianceIR2LcdNumber"));

        verticalLayout_4->addWidget(varianceIR2LcdNumber);


        horizontalLayout_6->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        varianceIR3Label = new QLabel(widget);
        varianceIR3Label->setObjectName(QStringLiteral("varianceIR3Label"));
        varianceIR3Label->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(varianceIR3Label);

        varianceIR3LcdNumber = new QLCDNumber(widget);
        varianceIR3LcdNumber->setObjectName(QStringLiteral("varianceIR3LcdNumber"));

        verticalLayout_3->addWidget(varianceIR3LcdNumber);


        horizontalLayout_6->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        varianceIR4Label = new QLabel(widget);
        varianceIR4Label->setObjectName(QStringLiteral("varianceIR4Label"));
        varianceIR4Label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(varianceIR4Label);

        varianceIR4LcdNumber = new QLCDNumber(widget);
        varianceIR4LcdNumber->setObjectName(QStringLiteral("varianceIR4LcdNumber"));

        verticalLayout_2->addWidget(varianceIR4LcdNumber);


        horizontalLayout_6->addLayout(verticalLayout_2);


        verticalLayout_8->addLayout(horizontalLayout_6);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        meanIR0LcdNumber = new QLCDNumber(widget);
        meanIR0LcdNumber->setObjectName(QStringLiteral("meanIR0LcdNumber"));

        horizontalLayout->addWidget(meanIR0LcdNumber);

        meanIR1LcdNumber = new QLCDNumber(widget);
        meanIR1LcdNumber->setObjectName(QStringLiteral("meanIR1LcdNumber"));

        horizontalLayout->addWidget(meanIR1LcdNumber);

        meanIR2LcdNumber = new QLCDNumber(widget);
        meanIR2LcdNumber->setObjectName(QStringLiteral("meanIR2LcdNumber"));

        horizontalLayout->addWidget(meanIR2LcdNumber);

        meanIR3LcdNumber = new QLCDNumber(widget);
        meanIR3LcdNumber->setObjectName(QStringLiteral("meanIR3LcdNumber"));

        horizontalLayout->addWidget(meanIR3LcdNumber);

        meanIR4LcdNumber = new QLCDNumber(widget);
        meanIR4LcdNumber->setObjectName(QStringLiteral("meanIR4LcdNumber"));

        horizontalLayout->addWidget(meanIR4LcdNumber);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        xiLabel = new QLabel(widget);
        xiLabel->setObjectName(QStringLiteral("xiLabel"));

        horizontalLayout_2->addWidget(xiLabel);

        ximeanIR0LcdNumber = new QLCDNumber(widget);
        ximeanIR0LcdNumber->setObjectName(QStringLiteral("ximeanIR0LcdNumber"));

        horizontalLayout_2->addWidget(ximeanIR0LcdNumber);

        ximeanIR1LcdNumber = new QLCDNumber(widget);
        ximeanIR1LcdNumber->setObjectName(QStringLiteral("ximeanIR1LcdNumber"));

        horizontalLayout_2->addWidget(ximeanIR1LcdNumber);

        ximeanIR2LcdNumber = new QLCDNumber(widget);
        ximeanIR2LcdNumber->setObjectName(QStringLiteral("ximeanIR2LcdNumber"));

        horizontalLayout_2->addWidget(ximeanIR2LcdNumber);

        ximeanIR3LcdNumber = new QLCDNumber(widget);
        ximeanIR3LcdNumber->setObjectName(QStringLiteral("ximeanIR3LcdNumber"));

        horizontalLayout_2->addWidget(ximeanIR3LcdNumber);

        ximeanIR4LcdNumber = new QLCDNumber(widget);
        ximeanIR4LcdNumber->setObjectName(QStringLiteral("ximeanIR4LcdNumber"));

        horizontalLayout_2->addWidget(ximeanIR4LcdNumber);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        ximeansqLabel = new QLabel(widget);
        ximeansqLabel->setObjectName(QStringLiteral("ximeansqLabel"));

        horizontalLayout_3->addWidget(ximeansqLabel);

        ximeansqIR0LcdNumber = new QLCDNumber(widget);
        ximeansqIR0LcdNumber->setObjectName(QStringLiteral("ximeansqIR0LcdNumber"));

        horizontalLayout_3->addWidget(ximeansqIR0LcdNumber);

        ximeansqIR1LcdNumber = new QLCDNumber(widget);
        ximeansqIR1LcdNumber->setObjectName(QStringLiteral("ximeansqIR1LcdNumber"));

        horizontalLayout_3->addWidget(ximeansqIR1LcdNumber);

        ximeansqIR2LcdNumber = new QLCDNumber(widget);
        ximeansqIR2LcdNumber->setObjectName(QStringLiteral("ximeansqIR2LcdNumber"));

        horizontalLayout_3->addWidget(ximeansqIR2LcdNumber);

        ximeansqIR3LcdNumber = new QLCDNumber(widget);
        ximeansqIR3LcdNumber->setObjectName(QStringLiteral("ximeansqIR3LcdNumber"));

        horizontalLayout_3->addWidget(ximeansqIR3LcdNumber);

        ximeansqIR4LcdNumber = new QLCDNumber(widget);
        ximeansqIR4LcdNumber->setObjectName(QStringLiteral("ximeansqIR4LcdNumber"));

        horizontalLayout_3->addWidget(ximeansqIR4LcdNumber);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        sumximeansqLabel = new QLabel(widget);
        sumximeansqLabel->setObjectName(QStringLiteral("sumximeansqLabel"));

        horizontalLayout_4->addWidget(sumximeansqLabel);

        sumximeansqIR0LcdNumber = new QLCDNumber(widget);
        sumximeansqIR0LcdNumber->setObjectName(QStringLiteral("sumximeansqIR0LcdNumber"));

        horizontalLayout_4->addWidget(sumximeansqIR0LcdNumber);

        sumximeansqIR1LcdNumber = new QLCDNumber(widget);
        sumximeansqIR1LcdNumber->setObjectName(QStringLiteral("sumximeansqIR1LcdNumber"));

        horizontalLayout_4->addWidget(sumximeansqIR1LcdNumber);

        sumximeansqIR2LcdNumber = new QLCDNumber(widget);
        sumximeansqIR2LcdNumber->setObjectName(QStringLiteral("sumximeansqIR2LcdNumber"));

        horizontalLayout_4->addWidget(sumximeansqIR2LcdNumber);

        sumximeansqIR3LcdNumber = new QLCDNumber(widget);
        sumximeansqIR3LcdNumber->setObjectName(QStringLiteral("sumximeansqIR3LcdNumber"));

        horizontalLayout_4->addWidget(sumximeansqIR3LcdNumber);

        sumximeansqIR4LcdNumber = new QLCDNumber(widget);
        sumximeansqIR4LcdNumber->setObjectName(QStringLiteral("sumximeansqIR4LcdNumber"));

        horizontalLayout_4->addWidget(sumximeansqIR4LcdNumber);


        verticalLayout->addLayout(horizontalLayout_4);


        verticalLayout_8->addLayout(verticalLayout);


        retranslateUi(varianceForm);

        QMetaObject::connectSlotsByName(varianceForm);
    } // setupUi

    void retranslateUi(QWidget *varianceForm)
    {
        varianceForm->setWindowTitle(QApplication::translate("varianceForm", "Form", 0));
        calculateVarianceButton->setText(QApplication::translate("varianceForm", "Calclaute Variance", 0));
        varianceStartedLabel->setText(QApplication::translate("varianceForm", "Variance Started:", 0));
        variance_started_label->setText(QApplication::translate("varianceForm", "No", 0));
        varianceIR0Label->setText(QApplication::translate("varianceForm", "Variance IR_0:", 0));
        varianceIR1Label->setText(QApplication::translate("varianceForm", "Variance IR_1:", 0));
        varianceIR2Label->setText(QApplication::translate("varianceForm", "Variance IR_2:", 0));
        varianceIR3Label->setText(QApplication::translate("varianceForm", "Variance IR_3:", 0));
        varianceIR4Label->setText(QApplication::translate("varianceForm", "Variance IR_4:", 0));
        label_4->setText(QApplication::translate("varianceForm", "Mean:", 0));
        xiLabel->setText(QApplication::translate("varianceForm", "Xi-Mean:", 0));
        ximeansqLabel->setText(QApplication::translate("varianceForm", "(Xi-Mean)^2 :", 0));
        sumximeansqLabel->setText(QApplication::translate("varianceForm", "Sum (Xi-Mean)^2:", 0));
    } // retranslateUi

};

namespace Ui {
    class varianceForm: public Ui_varianceForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VARIANCEFORM_H
