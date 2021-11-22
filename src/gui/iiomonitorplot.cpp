#include "iiomonitorplot.hpp"
#include "ui_iiomonitorplot.h"

#include <QPen>
#include "qwt_scale_engine.h"

using namespace adiscope;

IioMonitorPlot::IioMonitorPlot(QWidget *parent) :
	QwtPlot(parent)
	, sampleRate(10)
	, m_currentScale(-Q_INFINITY)
	, m_currentMaxValue(-Q_INFINITY)
{

	enableAxis(QwtPlot::xBottom,true);
//	enableAxis(QwtPlot::xTop,true);
	enableAxis(QwtPlot::yLeft,true);
	setAxisTitle(QwtPlot::xTop,tr("X axis"));
	setAxisTitle(QwtPlot::yLeft,tr("Y axis"));

	QVector<QwtScaleDiv> divs;
	QwtScaleEngine *engine = axisScaleEngine(QwtPlot::yLeft);
	engine->setAttribute(QwtScaleEngine::Inverted);
	divs.push_back(engine->divideScale(-0.1, +0.1, 5, 5));

	scaler = new AutoScaler(this, divs);


	connect(scaler, SIGNAL(updateScale(const QwtScaleDiv)),
			this, SLOT(updateScale(const QwtScaleDiv)));


	setNumSamples(100);
	curve.attach(this);
	curve.setYAxis(QwtPlot::yLeft);

	QPen pen(curve.pen());
	pen.setColor("blue");
	curve.setPen(pen);
}

void IioMonitorPlot::updatePlot(QMap<QString,double> updateValues){


	QMap<QString, QVector<double>>::iterator i;
	QMap<QString, double>::iterator j;


	QVector<double> indexes(m_curves[updateValues.begin().key()].size() + 1);
	std::iota(std::begin(indexes), std::end(indexes), 0);

	for(j = updateValues.begin(); j != updateValues.end(); ++j){
		if(m_curves[j.key()].size() == 11){
			m_curves[j.key()].pop_front();
		}
		m_curves[j.key()].push_back(j.value());
		//autoscale(j.value());

	}



	for (i = m_curves.begin(); i != m_curves.end(); ++i){
		if(updateValues[i.key()] != 0){
			QVector<double> indexes(i.value().size() + 1);
			std::iota(std::begin(indexes), std::end(indexes), 0);

			//plot->unregisterReferenceWaveform(i.key());
		//	plot->registerReferenceWaveform(i.key(), indexes, i.value());



		}else if(!i.value().isEmpty() ){
			i.value().pop_front();
			QVector<double> indexes(i.value().size() + 1);
			std::iota(std::begin(indexes), std::end(indexes), 0);

			//plot->unregisterReferenceWaveform(i.key());
			//plot->registerReferenceWaveform(i.key(), indexes, i.value());
		}

	}



	curve.setRawSamples(m_curves.begin().value().data(), yData.data() + (yData.size() -
				m_curves.begin().value().size()), m_curves.begin().value().size());

	replot();
}

double IioMonitorPlot::findMaxInFifo(){
	double max = -Q_INFINITY;
	for(int i = 0 ; i < xData.size(); i++){
		if(xData.at(i) > max){
			max = xData.at(i);
		}
	}
	return max;
}

void IioMonitorPlot::autoscale(){

	double val = findMaxInFifo();
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

//		setPlotAxisXTitle(formatedPrefix + m_unitOfMeasureName + "(" + formatedPrefix + m_unitOfMeasureSymbol + ")");
//		scaleLabel->setUnitOfMeasure(m_unitOfMeasureSymbol);


		QwtScaleEngine *scaleEngine = axisScaleEngine(QwtPlot::xTop);

		updateScale(scaleEngine->divideScale((-1*scale),scale,5,10));
		//QwtScaleDiv div = scaleEngine->divideScale((-1*scale),scale,5,10);
		//setAxisScale(QwtPlot::xTop, div.lowerBound(), div.upperBound());

		m_currentScale = scale;
	}
}

void IioMonitorPlot::updateScale(const QwtScaleDiv div)
{
	setAxisScale(QwtPlot::xTop, div.lowerBound(), div.upperBound());
   // setAxisScaleDraw(QwtPlot::xTop,scaleLabel);
}

void IioMonitorPlot::setXAxisScale(double min, double max){
	setXAxisScale(min,max);
}
void IioMonitorPlot::setYAxisScale(double min, double max){
//	setYAxisScale(min,max);
}

void IioMonitorPlot::setXAxisUnit(QString unit){
	setXAxisUnit(unit);
}
void IioMonitorPlot::setYAxisUnit(QString unit){

	setYAxisUnit(unit);
}


void IioMonitorPlot::plot(double sample)
{
	xData.push_back(sample);

	if (xData.size() == numSamples + 2){
		double aux = xData.first();

		if(aux == m_currentMaxValue){
			autoscale();
		}

			xData.pop_front();
	}

	if(sample > m_currentMaxValue){
		autoscale();
	}

	scaler->setValue(sample);


	curve.setRawSamples(xData.data(), yData.data() + (yData.size() -
				xData.size()), xData.size());
	replot();
}


int IioMonitorPlot::getNumSamples() const
{
	return numSamples;
}

void IioMonitorPlot::setNumSamples(int num)
{
	numSamples = (unsigned int) num;

	reset();
	yData.resize(numSamples + 1);
	xData.reserve(numSamples + 1);

	setAxisScale(QwtPlot::yLeft, (double) numSamples / sampleRate, 0.0);

	setSampleRate(sampleRate);
	replot();

	scaler->setTimeout((double) numSamples * 1000.0 / sampleRate);
}

void IioMonitorPlot::reset()
{
	xData.clear();
	scaler->startTimer();
}

double IioMonitorPlot::getSampleRate() const
{
	return sampleRate;
}

void IioMonitorPlot::setSampleRate(double rate)
{
	sampleRate = rate;

	for (unsigned int i = 0; i <= numSamples; i++)
		yData[i] = (double)(numSamples - i) / sampleRate;
}



IioMonitorPlot::~IioMonitorPlot()
{
	delete scaler;
}
