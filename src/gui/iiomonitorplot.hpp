#ifndef IIOMONITORPLOT_HPP
#define IIOMONITORPLOT_HPP


#include <QVector>
#include <QWidget>
#include <QMap>
#include <cmath>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include "plot_utils.hpp"
#include "autoScaler.hpp"
#include "gui/customqwtscaledraw.hpp"



namespace adiscope {
class IioMonitorPlot : public QwtPlot
{
	Q_OBJECT

	Q_PROPERTY(int numSamples
			READ getNumSamples
			WRITE setNumSamples
	)

	Q_PROPERTY(double sampleRate
			READ getSampleRate
			WRITE setSampleRate
	)

public:
	explicit IioMonitorPlot(QWidget *parent = nullptr);
	~IioMonitorPlot();

	int getNumSamples() const;
	void setNumSamples(int num);

	double getSampleRate() const;
	void setSampleRate(double rate);

	void updatePlot(QMap<QString,double> updateValues);

	void setXAxisUnit(QString unit);
	void setYAxisUnit(QString unit);
	void setXAxisScale(double min, double max);
	void setYAxisScale(double min, double max);


public Q_SLOTS:
	void plot(double sample);

private:
	unsigned int numSamples;
	double sampleRate;
	QMap<QString, QVector<double>> m_curves;
	QVector<double> yData;
	QVector<double> xData;
	double m_currentScale;
	double m_currentMaxValue;
	double findMaxInFifo();
	void autoscale();
	void updateScale(const QwtScaleDiv);
	void reset();

	QwtPlotCurve curve;
	AutoScaler *scaler;
};
}
#endif // IIOMONITORPLOT_HPP
