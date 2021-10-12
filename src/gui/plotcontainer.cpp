#include "plotcontainer.hpp"
#include "ui_plotcontainer.h"
#include <QGridLayout>

#include "logicanalyzer/logicdatacurve.h"

using namespace adiscope;
using namespace libm2k::context;
using namespace libm2k::analog;
using namespace gr;

PlotContainer::PlotContainer(libm2k::context::M2k *ctx, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotContainer),
    m_m2k_context(ctx),
    m_m2k_analogout(m_m2k_context->getAnalogOut()),
    m_currentScale(-Q_INFINITY)
{
    ui->setupUi(this);

    plot = new adiscope::FftDisplayPlot(2, this);
    plot->disableLegend();
    plot->setXaxisMouseGesturesEnabled(false);
    for (uint i = 0; i < 2; i++)
        plot->setYaxisMouseGesturesEnabled(i, false);
    plot->setZoomerEnabled();


    unsigned int noPoints = 100;
    unsigned int sampleRate = 1024;

    /*
    QVector<double> data;
    unsigned int maxValue = 99;

    for(unsigned int j = 0; j < noPoints; j++) {
           //data.push_back((std::rand() * (0 + 1)) % 100 );
        data.push_back(j%100);
    }

    QVector<double> indexes(1024);
    std::iota(std::begin(indexes), std::end(indexes), 0);

    plot->registerReferenceWaveform("1", indexes, data);

    plot->Curve(0)->setAxes(
           QwtAxisId(QwtPlot::xBottom, 0),
           QwtAxisId(QwtPlot::yLeft, 0));
    plot->Curve(0)->setTitle("CH " + QString::number(1));

    */
    /* Default plot settings */

    plot->setSampleRate(sampleRate, 1, "");

    plot->setHorizUnitsPerDiv(noPoints/10.0);
    plot->setVertUnitsPerDiv(100/10.0);

    //     plot->setHorizOffset(noPoints/2.0);
    //     plot->setVertOffset(maxValue/2.0);

    //set plot axis max values
    plot->setYaxis(-100, 100);
    plot->setXaxis(0, 10);


    //set unit of measure

    //     plot->setBtmHorAxisUnit("Hz");
    //   plot->setLeftVertAxisUnit("dB");
}



void PlotContainer::setPlotData(double data, QString plotCurveId){
    //   QVector<double> indexes(data.size());
    //  std::iota(std::begin(indexes), std::end(indexes), 0);

    //  plot->registerReferenceWaveform("", indexes, data);



    //    QVector<double> x;
    //    for(unsigned int j = 0; j < m_plotPoints.size(); j++) {
    //           //data.push_back((std::rand() * (0 + 1)) % 100 );
    //        x.push_back(m_plotPoints.at(j) + 10);
    //    }

    m_curves[plotCurveId].push_back(data);

    QVector<double> indexes(m_curves[plotCurveId].size() + 1);
    std::iota(std::begin(indexes), std::end(indexes), 0);

    if(m_curves[plotCurveId].size() == 11){
        //TODO pop from all ?
        //  m_curves[plotCurveId].pop_front();

        QMap<QString, QVector<double>>::iterator i;

        for (i = m_curves.begin(); i != m_curves.end(); ++i){
            if(!i.value().isEmpty()){
                i.value().pop_front();
                QVector<double> indexes(i.value().size() + 1);
                std::iota(std::begin(indexes), std::end(indexes), 0);

                plot->unregisterReferenceWaveform(i.key());
                plot->registerReferenceWaveform(i.key(), indexes, i.value());
            }
        }

    }

    // m_plotPoints.push_back(data);

    //    m_curves[plotCurveId].push_back(data);



    //    plot->unregisterReferenceWaveform(plotCurveId);
    //    plot->registerReferenceWaveform(plotCurveId, indexes, m_curves[plotCurveId]);

    plot->replot();

}

void PlotContainer::updatePlot(QMap<QString,double> updateValues){


    QMap<QString, QVector<double>>::iterator i;
    QMap<QString, double>::iterator j;


    QVector<double> indexes(m_curves[updateValues.begin().key()].size() + 1);
    std::iota(std::begin(indexes), std::end(indexes), 0);

    for(j = updateValues.begin(); j != updateValues.end(); ++j){
        if(m_curves[j.key()].size() == 11){
            m_curves[j.key()].pop_front();
        }
        m_curves[j.key()].push_back(j.value());
        autoscaleY(j.value());
    }



    for (i = m_curves.begin(); i != m_curves.end(); ++i){
        if(updateValues[i.key()] != 0){
            QVector<double> indexes(i.value().size() + 1);
            std::iota(std::begin(indexes), std::end(indexes), 0);

            plot->unregisterReferenceWaveform(i.key());
            plot->registerReferenceWaveform(i.key(), indexes, i.value());
        }else if(!i.value().isEmpty() ){
            i.value().pop_front();
            QVector<double> indexes(i.value().size() + 1);
            std::iota(std::begin(indexes), std::end(indexes), 0);

            plot->unregisterReferenceWaveform(i.key());
            plot->registerReferenceWaveform(i.key(), indexes, i.value());
        }

    }



    plot->replot();
}

void PlotContainer::autoscaleY(double val){

    int digits = 0;
    double num = val;
    if(int(num) == 0) {
        while((int)num*10 == 0){
            num *= 10;
            digits++;
        }
        digits = -1 * (digits - 1);
    }else{
        while((int)num){
            num /= 10;
            digits++;
        }
    }

    double scale = pow(10 , digits);

    if(scale > m_currentScale){

        MetricPrefixFormatter m_prefixFormater;
        QString formatedPrefix = m_prefixFormater.getFormatedMeasureUnit(val);

      //  setYAxisUnit(formatedPrefix);

        setYAxisScale(-scale,scale);

        m_currentScale = scale;
    }
}

void PlotContainer::setXAxisScale(double min, double max){
    plot->setXaxis(min, max);
}
void PlotContainer::setYAxisScale(double min, double max){
      plot->setYaxis(min, max);
}

void PlotContainer::setXAxisUnit(QString unit){
    plot->setBtmHorAxisUnit(unit);
}
void PlotContainer::setYAxisUnit(QString unit){

    plot->setLeftVertAxisUnit(unit);
}

PlotContainer::~PlotContainer()
{
    delete ui;
}
