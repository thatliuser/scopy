#include "channelmonitorcomponent.hpp"
#include "ui_channelmonitorcomponent.h"

using namespace adiscope;

ChannelMonitorComponent::ChannelMonitorComponent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelMonitorComponent),
    m_minValue(Q_INFINITY),
    m_maxValue(-Q_INFINITY)
{
    ui->setupUi(this);

}

void ChannelMonitorComponent::init(double value,QString unitOfMeasure, QString title){

    //updateValue(value,unitOfMeasure);
    ui->scaleCh1->setOrientation(Qt::Horizontal);
    ui->scaleCh1->setScalePosition(QwtThermo::LeadingScale);
    ui->scaleCh1->setOriginMode(QwtThermo::OriginCustom);
    ui->scaleCh1->setObjectName("IIOMonitor");
    ui->monitorTitle->setText(title);
    ui->sismograph_ch1_2->setColor(QColor("#ff7200"));

    ui->labelCh1->setText(unitOfMeasure);
    ui->sismograph_ch1_2->setPlotAxisXTitle(unitOfMeasure);

    ui->lcdCh1->setPrecision(3);

}

void ChannelMonitorComponent::updateValue(double value, QString unitOfMeasure){

   MetricPrefixFormatter m_prefixFormater;
   QString formatedPrefix = m_prefixFormater.getFormatedMeasureUnit(value);
   double formatedValue = m_prefixFormater.getFormatedValue(value);

    ui->lcdCh1->display(formatedValue);
    ui->labelCh1->setText(formatedPrefix + unitOfMeasure);

    ui->scaleCh1->setValue(formatedValue);

    ui->sismograph_ch1_2->setUnitOfMeasure(unitOfMeasure);
    ui->sismograph_ch1_2->plot(value);
    checkPeakValues(value, unitOfMeasure);
}


void ChannelMonitorComponent::checkPeakValues(double value,QString unitOfMeasure){
    MetricPrefixFormatter m_prefixFormater;
    QString formatedPrefix = m_prefixFormater.getFormatedMeasureUnit(value);
    double formatedValue = m_prefixFormater.getFormatedValue(value);

    if(value < m_minValue){
        m_minValue = value;

        ui->minCh1->display(formatedValue);
        ui->label_min->setText(formatedPrefix + unitOfMeasure + " MIN");
    }
    if(value > m_maxValue){
        m_maxValue = value;

        ui->maxCh1->display(formatedValue);
        ui->label_max->setText(formatedPrefix + unitOfMeasure + " MAX");
    }
}

void ChannelMonitorComponent::resetPeakHolder(){
    m_minValue = Q_INFINITY;
    ui->minCh1->display(0);
    m_maxValue = -Q_INFINITY;
    ui->maxCh1->display(0);
}

void ChannelMonitorComponent::displayPeakHold(bool checked){
    if(checked){
        ui->minCh1->show();
        ui->label_min->show();
        ui->maxCh1->show();
        ui->label_max->show();
    }else{
        ui->minCh1->hide();
        ui->label_min->hide();
        ui->maxCh1->hide();
        ui->label_max->hide();
    }
}

void ChannelMonitorComponent::displayHistory(bool checked){
    if(checked){
        ui->sismograph_ch1_2->show();
    }else{
        ui->sismograph_ch1_2->hide();
    }
}

void ChannelMonitorComponent::displayScale(bool checked){
    if(checked){
        ui->scaleCh1->show();
    }else {
        ui->scaleCh1->hide();
    }
}
std::string ChannelMonitorComponent::getChannelId(){ return m_channelId;}

void ChannelMonitorComponent::setChannelId(std::string channelId){m_channelId = channelId;}

void ChannelMonitorComponent::updateUnitOfMeasure(QString unitOfMeasure){
    m_unitOfMeasure = unitOfMeasure;
    ui->labelCh1->setText(unitOfMeasure);
}
QString ChannelMonitorComponent::getUnitOfMeasure(){return m_unitOfMeasure;}

void ChannelMonitorComponent::updateLcdNumberPrecision(int precision){
    ui->lcdCh1->setPrecision(precision);
    ui->lcdCh1->setDigitCount(4 + precision);
    ui->lcdCh1->display(ui->lcdCh1->value());

    ui->minCh1->setPrecision(precision);
    ui->minCh1->setDigitCount(4 + precision);


    ui->maxCh1->setPrecision(precision);
    ui->maxCh1->setDigitCount(4 + precision);

    if(m_minValue != Q_INFINITY){
    ui->minCh1->display(m_minValue);
    }
    if(m_maxValue != -Q_INFINITY){
    ui->maxCh1->display(m_maxValue);
    }
}

ChannelMonitorComponent::~ChannelMonitorComponent()
{
    delete ui;
}
