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

	m_plot = new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter);

	QWidget* plotWidget = new QWidget(this);
		  auto gridPlot = new QGridLayout(plotWidget);
		  gridPlot->setVerticalSpacing(0);
		  gridPlot->setHorizontalSpacing(0);
		  gridPlot->setContentsMargins(0, 0, 0, 0);
		  plotWidget->setLayout(gridPlot);
		  gridPlot->addWidget(m_plot, 3, 1, 1, 1);
		  gridPlot->addWidget(m_plot->leftHandlesArea(), 1, 0, 4, 1);

	ui->gridLayout_plot_2->addWidget(plotWidget);

	m_plot->disableLegend();
	m_plot->setXaxisMouseGesturesEnabled(false);
	for (uint i = 0; i < 2; i++)
		m_plot->setYaxisMouseGesturesEnabled(i, false);

	//TODO compute sample rate
	setNumSamples(100);
	setSampleRate(0.2);

	/* Default plot settings */
	m_plot->setYaxis(-100, 100);
	m_plot->setHorizUnitsPerDiv(10);
	m_plot->setXaxis(-100, 0);
	m_plot->enableTimeTrigger(false);
	m_plot->setYaxisUnit("");

}

CapturePlot* PlotContainer::getPlot(){ return  m_plot;}

/*
This function will receive a map of values to update plot curves, this map will use curveId as a key
For each curve the map contains: new value to add to curve and curve color
For curves that receive values curve is updated
For curves that don't receive new values NAN value is added (to push curve on plot)
Curves that only contain NAN values are removed
*/
void PlotContainer::updatePlot(QMap<int,QPair<double,QColor>> updateValues){
	QMap<int, QVector<double>>::iterator curveMapIterator;
	QMap<int, QPair<double,QColor>>::iterator updateValIterator;


	for(updateValIterator = updateValues.begin(); updateValIterator != updateValues.end(); ++updateValIterator){
		m_curves[updateValIterator.key()].push_front(updateValIterator.value().first);
		if(m_curves[updateValIterator.key()].size() == numSamples + 2){
			m_curves[updateValIterator.key()].pop_back();
		}
	}

	for (curveMapIterator = m_curves.begin(); curveMapIterator != m_curves.end(); curveMapIterator++){
		if(updateValues.contains(curveMapIterator.key())){//[curveMapIterator.key()].first != 0){
			updatePlotCurve(curveMapIterator.key(), xdata, curveMapIterator.value(), updateValues[curveMapIterator.key()].second);
		}else
		{
			curveMapIterator.value().push_front(NAN);
			if(curveMapIterator.value().size() == numSamples + 1)
			{
				if(checkEmptyCurve(curveMapIterator.key())) //curveMapIterator.value().first() == NAN)
				{
					m_curves.erase(curveMapIterator);
					plotManager.erase(plotManager.find(curveMapIterator.key()));
					m_plot->DetachCurve(curveMapIterator.key());
					break;
				}else{
					curveMapIterator.value().pop_back();
					updatePlotCurve(curveMapIterator.key(), xdata, curveMapIterator.value(),updateValues[curveMapIterator.key()].second);
				}
			}else{
				updatePlotCurve(curveMapIterator.key(), xdata, curveMapIterator.value(),updateValues[curveMapIterator.key()].second);
			}
		}
	}
	m_plot->replot();
}

bool PlotContainer::checkEmptyCurve(int key){
	for(int i = 0; i < m_curves[key].size(); i++ ){
		if(!isnan(m_curves[key].at(i))){
			return false;
		}
	}
	return true;
}

void PlotContainer::updatePlotCurve(int key, QVector<double> xdata, QVector<double> ydata, QColor color){

	if(plotManager[key] == 0){
		//if no curve exists for this key create new curve

		m_plot->setLeftVertAxesCount(key + 1);

		QwtPlotCurve *curve = new QwtPlotCurve();
		curve->attach(m_plot);
		curve->setPen(QPen(color));
		curve->setAxes(QwtAxisId(QwtPlot::xBottom, 0), QwtAxisId(QwtPlot::yLeft, key));

		plotManager[key] = curve;
		//last added curve is active curve
		m_plot->setActiveVertAxis(key,true);
		m_plot->replot();

		//Q_EMIT plot->channelAdded(key);

		// TODO will use autoscale from Teo
//		m_plot->setAxisAutoScale(QwtAxisId(QwtPlot::yLeft,key),true);


	}
	// update values from curve
//	if(!isnan(ydata.first()))
//		autoscaleY(ydata.first(), key);
	plotManager[key]->setSamples(xdata,ydata);
}


void PlotContainer::bringCurveToFront(int curveKey){
	m_plot->bringCurveToFront(curveKey);
}

void PlotContainer::setAxisActive(int curveKey){
	m_plot->setActiveVertAxis(curveKey,true);
}

// TODO will use autoscale from Teo
void PlotContainer::autoscaleY(double val, QString key){

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

		//plot->setYaxis(-scale,scale);

		//plot->setAxisScale(QwtAxisId(QwtPlot::yLeft, plotManager[key].first),-scale,scale);
		//plot->setAxisScale(plotManager[key].first,-scale,scale,10);
		//plot->replot();
		m_currentScale = scale;
	}
}

void PlotContainer::setXAxisScale(double min, double max){ 	m_plot->setXaxis(min, max); }
void PlotContainer::setYAxisScale(double min, double max){	m_plot->setYaxis(min, max);}

void PlotContainer::setHistoryDuration(double time){
	// TODO test if for TimePrefixFormater numa sample = time
	numSamples = (unsigned int) time * sampleRate;
	setNumSamples(numSamples);
}

void PlotContainer::setNumSamples(int num)
{
	numSamples = (unsigned int) num;
	xdata.resize(numSamples + 1);
	m_plot->setAxisScale(QwtPlot::xBottom,(-1)*numSamples / sampleRate, 0.0);
	setSampleRate(sampleRate);
	m_plot->replot();
}

double PlotContainer::getSampleRate() const{ return sampleRate;}

void PlotContainer::setSampleRate(double rate)
{
	sampleRate = rate;
	for (unsigned int i = 0; i <= numSamples; i++)
		xdata[numSamples - i] = (-1) * (double)(numSamples - i) / sampleRate;
}


PlotContainer::~PlotContainer()
{
	delete ui;
}
