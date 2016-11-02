#include "grippersimulator.h"
#include "ui_grippersimulator.h"

grippersimulator::grippersimulator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::grippersimulator)
{
    ui->setupUi(this);

    ui->distanceSlider->setMinimum(0);
    ui->distanceSlider->setMaximum(10);
    ui->distanceSlider->setValue(10);
    connect(ui->distanceSlider, SIGNAL(valueChanged(int)), ui->distanceLCD, SLOT(display(int)));

    ui->angleSlider->setMinimum(-90);
    ui->angleSlider->setMaximum(90);
    ui->angleSlider->setValue(0);
    connect(ui->angleSlider, SIGNAL(valueChanged(int)), ui->angleLCD, SLOT(display(int)));
}

grippersimulator::~grippersimulator()
{
    delete ui;
}

void grippersimulator::setConfig(initialization* config){
    this->config = config;

    ui->Alignment_Info->addItem("Not_Aligned");
    ui->Alignment_Info->addItem("Slight_Right");
    ui->Alignment_Info->addItem("Slight_Left");
    ui->Alignment_Info->addItem("Major_Right");
    ui->Alignment_Info->addItem("Major_Left");
    ui->Alignment_Info->addItem("Aligned_Not_Close");
    ui->Alignment_Info->addItem("Aligned");
    ui->Alignment_Info->addItem("Obstacle");

    on_targetAlignment_clicked(ui->targetAlignment->isChecked());
    on_Grabber_Aligned_clicked(ui->Grabber_Aligned->isChecked());
    on_distanceSlider_sliderMoved(ui->distanceSlider->value());
    on_angleSlider_sliderMoved(ui->angleSlider->value());

}

void grippersimulator::on_targetAlignment_clicked(bool checked)
{
    QJsonObject object
    {
        {"targetAcquired", "checked"}
    };
    object["targetAcquired"] = checked ? "true" : "false" ;
    config->changeResponse(object);

}

void grippersimulator::on_Grabber_Aligned_clicked(bool checked)
{
    targetAlig["Grabber_Aligned"] = checked ? "true" : "false";
    QJsonObject container;
    container["targetAlignment"] = targetAlig;
    config->changeResponse(container);
    qDebug() << config->getConfig("responses.json")["targetAlignment"].toObject()["Grabber_Aligned"].toString();
}

void grippersimulator::on_distanceSlider_sliderMoved(int position)
{
    targetAlig["Target_Distance"] = QString::number(position);
    QJsonObject container;
    container["targetAlignment"] = targetAlig;
    config->changeResponse(container);
}

void grippersimulator::on_angleSlider_sliderMoved(int position)
{
    targetAlig["Target_Angle"] = QString::number(position);
    QJsonObject container;
    container["targetAlignment"] = targetAlig;
    config->changeResponse(container);
}

void grippersimulator::on_Alignment_Info_currentTextChanged(const QString &arg1)
{
    targetAlig["Alignment_Info"] = arg1;
    QJsonObject container;
    container["targetAlignment"] = targetAlig;
    config->changeResponse(container);

    qDebug() << config->getConfig("responses.json")["targetAlignment"].toObject()["Target_Angle"].toString();
    qDebug() << config->getConfig("responses.json")["targetAlignment"].toObject()["Target_Distance"].toString();
    qDebug() << config->getConfig("responses.json")["targetAlignment"].toObject()["Grabber_Aligned"].toString();

    qDebug() << config->getConfig("responses.json")["targetAlignment"].toObject()["Alignment_Info"].toString();
}
