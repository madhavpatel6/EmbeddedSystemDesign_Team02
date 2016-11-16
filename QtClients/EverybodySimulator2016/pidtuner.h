#ifndef PIDTUNER_H
#define PIDTUNER_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsTextItem>


namespace Ui {
class pidTuner;
}

class pidTuner : public QWidget
{
    Q_OBJECT

public:
    explicit pidTuner(QWidget *parent = 0);

    ~pidTuner();
public slots:
    void motorUpdate(QJsonObject in);

signals:
    void upClicked();
    void downClicked();

private slots:
    void on_pushButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

private:
    Ui::pidTuner *ui;
    QGraphicsScene m1Scene;
    QGraphicsScene m2Scene;
    void drawDot(int motorNum, int x, int y);
};

#endif // PIDTUNER_H
