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

    CapturePlot *m_plot;

    void setPlotData(double data, QString plotCurveId);

    void updatePlot(QMap<QString,double> updateValues);

    void setXAxisUnit(QString unit);
    void setYAxisUnit(QString unit);
    void setXAxisScale(double min, double max);
    void setYAxisScale(double min, double max);

    void autoscaleY(double val);

private:
    Ui::PlotContainer *ui;
    libm2k::context::M2k* m_m2k_context;

    libm2k::analog::M2kAnalogOut* m_m2k_analogout;
    adiscope::FftDisplayPlot* plot;

    QVector<double> m_plotPoints;
    QMap<QString, QVector<double>> m_curves;

    double m_currentScale;
};
}
#endif // PLOTCONTAINER_HPP
