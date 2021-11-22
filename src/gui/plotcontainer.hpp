#ifndef PLOTCONTAINER_HPP
#define PLOTCONTAINER_HPP

#include <QWidget>

#include "oscilloscope_plot.hpp"
#include "plot_utils.hpp"
#include <QMainWindow>
#include "startstoprangewidget.h"
#include "FftDisplayPlot.h"


/* libm2k includes */
#include <libm2k/analog/m2kanalogin.hpp>
#include <libm2k/m2k.hpp>
#include <libm2k/analog/dmm.hpp>
#include <libm2k/contextbuilder.hpp>
#include <scopyExceptionHandler.h>
#include <libm2k/m2kexceptions.hpp>

namespace Ui {
class PlotContainer;
}
namespace adiscope {


class PlotContainer : public QWidget
{
	Q_OBJECT

public:
	explicit PlotContainer(libm2k::context::M2k *ctx, QWidget *parent = nullptr);
	~PlotContainer();


	void updatePlot(QMap<int,QPair<double,QColor>> updateValues);

	void setXAxisScale(double min, double max);
	void setYAxisScale(double min, double max);

	void autoscaleY(double val, QString key);

	void setHistoryDuration(double duration);
	int getNumSamples() const;
	void setNumSamples(int num);
	double getSampleRate() const;
	void setSampleRate(double rate);

	void bringCurveToFront(int curveKey);
	void setAxisActive(int curveKey);


	CapturePlot* getPlot();


private:
	Ui::PlotContainer *ui;
	libm2k::context::M2k* m_m2k_context;

	libm2k::analog::M2kAnalogOut* m_m2k_analogout;
	CapturePlot* m_plot;
	QVector<double> m_plotPoints;
	QMap<int, QVector<double>> m_curves;
	QMap<int, QwtPlotCurve*> plotManager;

	void updatePlotCurve(int key, QVector<double> xdata, QVector<double> ydata, QColor color);
	void removeCurveFromPlot(int key);
	bool checkEmptyCurve(int key);

	QVector<double> xdata;

	double m_currentScale;
	int numSamples;
	double sampleRate;
};
}
#endif // PLOTCONTAINER_HPP
