#ifndef TG_REQ_RESPFORM_H
#define TG_REQ_RESPFORM_H

#include <QWidget>

namespace Ui {
class tg_req_respForm;
}

class tg_req_respForm : public QWidget
{
    Q_OBJECT

public:
    explicit tg_req_respForm(QWidget *parent = 0);
    ~tg_req_respForm();

public slots:
    void updatetgCurReqSlot(QString curReq);
    void updatetgCurRespSlot(QString curResp);

private:
    void updatetgPrevReqSlot(QString prevReq);
    void updatetgPrevRespSlot(QString prevResp);
    QString tgprevReq;
    QString tgprevResp;
    Ui::tg_req_respForm *ui;
};

#endif // TG_REQ_RESPFORM_H
