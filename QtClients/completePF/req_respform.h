#ifndef REQ_RESPFORM_H
#define REQ_RESPFORM_H

#include <QWidget>

namespace Ui {
class req_respForm;
}

class req_respForm : public QWidget
{
    Q_OBJECT

public:
    explicit req_respForm(QWidget *parent = 0);
    ~req_respForm();

public slots:
    void updateCurReqSlot(QString curReq);
    void updateCurRespSlot(QString curResp);

private:
    void updatePrevReqSlot(QString prevReq);
    void updatePrevRespSlot(QString prevResp);
    QString prevReq;
    QString prevResp;
    Ui::req_respForm *ui;
};

#endif // REQ_RESPFORM_H
