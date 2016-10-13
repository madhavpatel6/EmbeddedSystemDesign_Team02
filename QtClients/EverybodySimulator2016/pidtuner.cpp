#include "pidtuner.h"
#include "ui_pidtuner.h"

pidTuner::pidTuner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pidTuner)
{
    ui->setupUi(this);

    QRectF dot(0,0,1, 1);
    QGraphicsRectItem *dotR = m1Scene.addRect(dot);
    ui->plotMotor1->setTransform(QTransform::fromScale(2, 2));
    ui->plotMotor1->setScene(&m1Scene);
    ui->plotMotor2->setTransform(QTransform::fromScale(2, 2));
    ui->plotMotor2->setScene(&m2Scene);

    QGraphicsTextItem *label = m1Scene.addText("0");
    label->setPos(QPointF(-10, 0));
    label = m1Scene.addText("10");
    label->setPos(QPointF(-10, -100));


    label = m2Scene.addText("0");
    label->setPos(QPointF(-10, 0));
    label = m2Scene.addText("10");
    label->setPos(QPointF(-10, -100));
}

void pidTuner::motorUpdate(QJsonObject in){
    qDebug() << QString(QJsonDocument(in).toJson());
    if(in["motor"] == "1"){
        ui->jsonMotor1->setText(QString(QJsonDocument(in).toJson()));
        int y = -1 * in["vel"].toString().toInt();
        int x = in["time"].toString().toInt();
        drawDot(1, x, y);
    }else if(in["motor"] == "2"){
        ui->jsonMotor2->setText(QJsonDocument(in).toJson());
        int y = -1 * in["vel"].toString().toInt();
        int x = in["time"].toString().toInt();
        drawDot(2, x, y);
    }
}

void pidTuner::drawDot(int motorNum, int x, int y){
    QRectF dot(x,y*10,1, 1);
    if(motorNum == 1){
        QGraphicsRectItem *dotR = m1Scene.addRect(dot);

    }else if(motorNum == 2){
        QGraphicsRectItem *dotR = m2Scene.addRect(dot);

    }
}

pidTuner::~pidTuner()
{
    delete ui;
}

void pidTuner::on_pushButton_clicked()
{
    m1Scene.clear();
    m2Scene.clear();

    QGraphicsTextItem *label = m1Scene.addText("0");
    label->setPos(QPointF(-10, 0));
    label = m1Scene.addText("10");
    label->setPos(QPointF(-10, -100));


    label = m2Scene.addText("0");
    label->setPos(QPointF(-10, 0));
    label = m2Scene.addText("10");
    label->setPos(QPointF(-10, -100));
}
