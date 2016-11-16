#include "varianceform.h"
#include "ui_varianceform.h"

varianceForm::varianceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::varianceForm)
{
    ui->setupUi(this);
    IR0RequestCounter = 0;
    IR1RequestCounter = 0;
    IR2RequestCounter = 0;
    IR3RequestCounter = 0;
    IR4RequestCounter = 0;
    IR0Meanvar = 0.0;
    IR1Meanvar = 0.0;
    IR2Meanvar = 0.0;
    IR3Meanvar = 0.0;
    IR4Meanvar = 0.0;
    IR0MeanSqSumvar = 0.0;
    IR1MeanSqSumvar = 0.0;
    IR2MeanSqSumvar = 0.0;
    IR3MeanSqSumvar = 0.0;
    IR4MeanSqSumvar = 0.0;
    IR0Sumvar = 0.0;
    IR1Sumvar = 0.0;
    IR2Sumvar = 0.0;
    IR3Sumvar = 0.0;
    IR4Sumvar = 0.0;
    IR0var = 0.0;
    IR1var = 0.0;
    IR2var = 0.0;
    IR3var = 0.0;
    IR4var = 0.0;
    varCounter = 0;
    varSumCounter = 0;
}

varianceForm::~varianceForm()
{
    IR0RequestCounter = 0;
    IR1RequestCounter = 0;
    IR2RequestCounter = 0;
    IR3RequestCounter = 0;
    IR4RequestCounter = 0;
    IR0Meanvar = 0.0;
    IR1Meanvar = 0.0;
    IR2Meanvar = 0.0;
    IR3Meanvar = 0.0;
    IR4Meanvar = 0.0;
    IR0MeanSqSumvar = 0.0;
    IR1MeanSqSumvar = 0.0;
    IR2MeanSqSumvar = 0.0;
    IR3MeanSqSumvar = 0.0;
    IR4MeanSqSumvar = 0.0;
    IR0Sumvar = 0.0;
    IR1Sumvar = 0.0;
    IR2Sumvar = 0.0;
    IR3Sumvar = 0.0;
    IR4Sumvar = 0.0;
    IR0var = 0.0;
    IR1var = 0.0;
    IR2var = 0.0;
    IR3var = 0.0;
    IR4var = 0.0;
    varCounter = 0;
    varSumCounter = 0;
    delete ui;
}

void varianceForm::updateVarianceData(char source, QString alignmentData0, QString alignmentData1, QString alignmentData2, QString alignmentData3, QString alignmentData4)
{
    float xi_meanIR0;
    float xi_meanIR1;
    float xi_meanIR2;
    float xi_meanIR3;
    float xi_meanIR4;
    float xi_meanSqIR0;
    float xi_meanSqIR1;
    float xi_meanSqIR2;
    float xi_meanSqIR3;
    float xi_meanSqIR4;
    if(alignmentData0 == "0.00"){
        IR0RequestCounter = IR0RequestCounter;
    }
    else{
        IR0RequestCounter = IR0RequestCounter + 1;
    }
    if(alignmentData1 == "0.00"){
        IR1RequestCounter = IR1RequestCounter;
    }
    else{
        IR1RequestCounter = IR1RequestCounter + 1;
    }
    if(alignmentData2 == "0.00"){
        IR2RequestCounter = IR2RequestCounter;
    }
    else{
        IR2RequestCounter = IR2RequestCounter + 1;
    }
    if(alignmentData3 == "0.00"){
        IR3RequestCounter = IR3RequestCounter;
    }
    else{
        IR3RequestCounter = IR3RequestCounter + 1;
    }
    if(alignmentData4 == "0.00"){
        IR4RequestCounter = IR4RequestCounter;
    }
    else{
        IR4RequestCounter = IR4RequestCounter + 1;
    }
    switch(source){
        case TARGETGRABBER:
            if(startVar == true){
                if(varCounter == 5){
                    IR0Meanvar = IR0Sumvar / (float)IR0RequestCounter;
                    IR1Meanvar = IR1Sumvar / (float)IR1RequestCounter;
                    IR2Meanvar = IR2Sumvar / (float)IR2RequestCounter;
                    IR3Meanvar = IR3Sumvar / (float)IR3RequestCounter;
                    IR4Meanvar = IR4Sumvar / (float)IR4RequestCounter;
                    ui->meanIR0LcdNumber->display(IR0Meanvar);
                    ui->meanIR1LcdNumber->display(IR1Meanvar);
                    ui->meanIR2LcdNumber->display(IR2Meanvar);
                    ui->meanIR3LcdNumber->display(IR3Meanvar);
                    ui->meanIR4LcdNumber->display(IR4Meanvar);
                    xi_meanIR0 = alignmentData0.toFloat() - IR0Meanvar;
                    xi_meanIR1 = alignmentData1.toFloat() - IR1Meanvar;
                    xi_meanIR2 = alignmentData2.toFloat() - IR2Meanvar;
                    xi_meanIR3 = alignmentData3.toFloat() - IR3Meanvar;
                    xi_meanIR4 = alignmentData4.toFloat() - IR4Meanvar;
                    ui->ximeanIR0LcdNumber->display(xi_meanIR0);
                    ui->ximeanIR1LcdNumber->display(xi_meanIR1);
                    ui->ximeanIR2LcdNumber->display(xi_meanIR2);
                    ui->ximeanIR3LcdNumber->display(xi_meanIR3);
                    ui->ximeanIR4LcdNumber->display(xi_meanIR4);
                    xi_meanSqIR0 = qPow(xi_meanIR0, 2);
                    xi_meanSqIR1 = qPow(xi_meanIR1, 2);
                    xi_meanSqIR2 = qPow(xi_meanIR2, 2);
                    xi_meanSqIR3 = qPow(xi_meanIR3, 2);
                    xi_meanSqIR4 = qPow(xi_meanIR4, 2);
                    ui->ximeansqIR0LcdNumber->display(xi_meanSqIR0);
                    ui->ximeansqIR1LcdNumber->display(xi_meanSqIR1);
                    ui->ximeansqIR2LcdNumber->display(xi_meanSqIR2);
                    ui->ximeansqIR3LcdNumber->display(xi_meanSqIR3);
                    ui->ximeansqIR4LcdNumber->display(xi_meanSqIR4);
                    if(varSumCounter == 5){
                        ui->sumximeansqIR0LcdNumber->display(IR0MeanSqSumvar);
                        ui->sumximeansqIR1LcdNumber->display(IR1MeanSqSumvar);
                        ui->sumximeansqIR2LcdNumber->display(IR2MeanSqSumvar);
                        ui->sumximeansqIR3LcdNumber->display(IR3MeanSqSumvar);
                        ui->sumximeansqIR4LcdNumber->display(IR4MeanSqSumvar);
                        IR0var = IR0MeanSqSumvar / ((float)IR0RequestCounter-1.0);
                        IR1var = IR1MeanSqSumvar / ((float)IR1RequestCounter-1.0);
                        IR2var = IR2MeanSqSumvar / ((float)IR2RequestCounter-1.0);
                        IR3var = IR3MeanSqSumvar / ((float)IR3RequestCounter-1.0);
                        IR4var = IR4MeanSqSumvar / ((float)IR4RequestCounter-1.0);
                        ui->varianceIR0LcdNumber->display(IR0var);
                        ui->varianceIR1LcdNumber->display(IR1var);
                        ui->varianceIR2LcdNumber->display(IR2var);
                        ui->varianceIR3LcdNumber->display(IR3var);
                        ui->varianceIR4LcdNumber->display(IR4var);
                        varSumCounter = 0;
                    }
                    else{
                        IR0MeanSqSumvar = IR0MeanSqSumvar + xi_meanSqIR0;
                        IR1MeanSqSumvar = IR1MeanSqSumvar + xi_meanSqIR1;
                        IR2MeanSqSumvar = IR2MeanSqSumvar + xi_meanSqIR2;
                        IR3MeanSqSumvar = IR3MeanSqSumvar + xi_meanSqIR3;
                        IR4MeanSqSumvar = IR4MeanSqSumvar + xi_meanSqIR4;
                        varSumCounter = varSumCounter + 1;
                    }
                    varCounter = 0;
                }
                else{
                    IR0Sumvar = IR0Sumvar + alignmentData0.toFloat();
                    IR1Sumvar = IR1Sumvar + alignmentData1.toFloat();
                    IR2Sumvar = IR2Sumvar + alignmentData2.toFloat();
                    IR3Sumvar = IR3Sumvar + alignmentData3.toFloat();
                    IR4Sumvar = IR4Sumvar + alignmentData4.toFloat();
                    varCounter = varCounter + 1;
                }
            }
            break;
    }
}


void varianceForm::on_calculateVarianceButton_clicked()
{
    if(startVar == true){
        ui->variance_started_label->setText(QString("No"));
        IR0Sumvar = 0.0;
        IR1Sumvar = 0.0;
        IR2Sumvar = 0.0;
        IR3Sumvar = 0.0;
        IR4Sumvar = 0.0;
        IR0MeanSqSumvar = 0.0;
        IR1MeanSqSumvar = 0.0;
        IR2MeanSqSumvar = 0.0;
        IR3MeanSqSumvar = 0.0;
        IR4MeanSqSumvar = 0.0;
        IR0var = 0.0;
        IR1var = 0.0;
        IR2var = 0.0;
        IR3var = 0.0;
        IR4var = 0.0;
        varCounter = 0;
        varSumCounter = 0;
        startVar = false;
    }
    else{
        ui->variance_started_label->setText(QString("Yes"));
        startVar = true;
    }
}
