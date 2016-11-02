#ifndef GRIPPERSIMULATOR_H
#define GRIPPERSIMULATOR_H

#include <QWidget>
#include "initialization.h"

namespace Ui {
class grippersimulator;
}

class grippersimulator : public QWidget
{
    Q_OBJECT

public:
    explicit grippersimulator(QWidget *parent = 0);
    ~grippersimulator();
    void setConfig(initialization* config);

private slots:
    void on_targetAlignment_clicked(bool checked);

    void on_Grabber_Aligned_clicked(bool checked);

    void on_distanceSlider_sliderMoved(int position);

    void on_angleSlider_sliderMoved(int position);

    void on_Alignment_Info_currentTextChanged(const QString &arg1);

private:
    Ui::grippersimulator *ui;
    initialization* config;
    QJsonObject targetAlig
    {
        {"Grabber_Aligned", "nothing"}
    };
};

#endif // GRIPPERSIMULATOR_H
