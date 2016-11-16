#ifndef VARIANCEFORM_H
#define VARIANCEFORM_H

#include <QWidget>
#include "qmath.h"
#include "picCode/communication/messages.h"

namespace Ui {
class varianceForm;
}

class varianceForm : public QWidget
{
    Q_OBJECT

public:
    explicit varianceForm(QWidget *parent = 0);
    ~varianceForm();

public slots:
    void updateVarianceData(char source, QString alignmentData0,
                            QString alignmentData1, QString alignmentData2,
                            QString alignmentData3, QString alignmentData4);

private slots:
    void on_calculateVarianceButton_clicked();

private:
    Ui::varianceForm *ui;
    int IR0RequestCounter;
    int IR1RequestCounter;
    int IR2RequestCounter;
    int IR3RequestCounter;
    int IR4RequestCounter;
    float IR0Meanvar;
    float IR1Meanvar;
    float IR2Meanvar;
    float IR3Meanvar;
    float IR4Meanvar;
    float IR0MeanSqSumvar;
    float IR1MeanSqSumvar;
    float IR2MeanSqSumvar;
    float IR3MeanSqSumvar;
    float IR4MeanSqSumvar;
    float IR0Sumvar;
    float IR1Sumvar;
    float IR2Sumvar;
    float IR3Sumvar;
    float IR4Sumvar;
    float IR0var;
    float IR1var;
    float IR2var;
    float IR3var;
    float IR4var;
    bool startVar;
    int varCounter;
    int varSumCounter;
};

#endif // VARIANCEFORM_H
