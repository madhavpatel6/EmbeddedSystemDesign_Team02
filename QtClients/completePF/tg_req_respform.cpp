#include "tg_req_respform.h"
#include "ui_tg_req_respform.h"

tg_req_respForm::tg_req_respForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tg_req_respForm)
{
    ui->setupUi(this);
    tgprevReq = "N/A";
    tgprevResp = "N/A";
}

tg_req_respForm::~tg_req_respForm()
{
    tgprevReq = "N/A";
    tgprevResp = "N/A";
    delete ui;
}

void tg_req_respForm::updatetgCurReqSlot(QString curReq){
    ui->tgcurReqPlainTextEdit->clear();
    ui->tgcurReqPlainTextEdit->appendPlainText(curReq);
    if(tgprevReq != curReq){
        updatetgPrevReqSlot(tgprevReq);
        tgprevReq = curReq;
    }
}

void tg_req_respForm::updatetgCurRespSlot(QString curResp){
    ui->tgcurRespPlainTextEdit->clear();
    ui->tgcurRespPlainTextEdit->appendPlainText(curResp);
    if(tgprevResp != curResp){
        updatetgPrevRespSlot(tgprevResp);
        tgprevResp = curResp;
    }
}

void tg_req_respForm::updatetgPrevReqSlot(QString prevReq){
    ui->tgprevReqPlainTextEdit->clear();
    ui->tgprevReqPlainTextEdit->appendPlainText(prevReq);
}

void tg_req_respForm::updatetgPrevRespSlot(QString prevResp){
    ui->tgprevRespPlainTextEdit->clear();
    ui->tgprevRespPlainTextEdit->appendPlainText(prevResp);
}
