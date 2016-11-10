#include "req_respform.h"
#include "ui_req_respform.h"

req_respForm::req_respForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::req_respForm)
{
    ui->setupUi(this);
    prevReq = "N/A";
    prevResp = "N/A";
}

req_respForm::~req_respForm()
{
    prevReq = "N/A";
    prevResp = "N/A";
    delete ui;
}

void req_respForm::updateCurReqSlot(QString curReq){
    ui->curReqPlainTextEdit->clear();
    ui->curReqPlainTextEdit->appendPlainText(curReq);
    if(prevReq != curReq){
        updatePrevReqSlot(prevReq);
        prevReq = curReq;
    }
}

void req_respForm::updateCurRespSlot(QString curResp){
    ui->curRespPlainTextEdit->clear();
    ui->curRespPlainTextEdit->appendPlainText(curResp);
    if(prevResp != curResp){
        updatePrevRespSlot(prevResp);
        prevResp = curResp;
    }
}

void req_respForm::updatePrevReqSlot(QString prevReq){
    ui->prevReqPlainTextEdit->clear();
    ui->prevReqPlainTextEdit->appendPlainText(prevReq);
}

void req_respForm::updatePrevRespSlot(QString prevResp){
    ui->prevRespPlainTextEdit->clear();
    ui->prevRespPlainTextEdit->appendPlainText(prevResp);
}

