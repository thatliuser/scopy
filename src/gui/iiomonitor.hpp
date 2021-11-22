#ifndef IIOMONITOR_HPP
#define IIOMONITOR_HPP

#include <QWidget>
#include <filter.hpp>
#include <tool_launcher.hpp>
#include "ui_iiomonitor.h"
#include <tool.hpp>
#include "gui/tool_view_builder.hpp"
#include <QPair>
#include <QTimer>
#include "gui/channelmonitorcomponent.hpp"
#include "gui/iiomonitormenu.hpp"
#include "gui/monitorcontainer.hpp"
#include "iiomonitorgeneralsettingsmenu.hpp"
#include "plotcontainer.hpp"
#include "iiomonitorplot.hpp"

#include "gui/iiomonitorgenericmenu.hpp"

/* libm2k includes */
#include <libm2k/analog/m2kanalogin.hpp>
#include <libm2k/m2k.hpp>
#include <libm2k/analog/dmm.hpp>
#include <libm2k/contextbuilder.hpp>
#include <scopyExceptionHandler.h>
#include <libm2k/m2kexceptions.hpp>

namespace adiscope {

struct activeChannel{
	std::string dmmId;
	libm2k::analog::DMM* dmm;
	int numberOfTabsUsing = 0;
	QColor chColor;

};

class IIOMonitor : public Tool
{
    Q_OBJECT

    Ui::IIOMonitor *ui;

public:
    explicit IIOMonitor(struct iio_context *ctx, Filter *filt,
                        ToolMenuItem *toolMenuItem,
                        QJSEngine *engine, ToolLauncher *parent);
    ~IIOMonitor();

    scopy::gui::ToolView* getToolView();

private:
	 int VALUE_READING_TIME_INTERVAL = 1000;

    libm2k::context::M2k* m_m2k_context;
	scopy::gui::ToolView* m_toolView;
    std::vector<libm2k::analog::DMM*> m_dmmList;
    std::vector<ChannelWidget*> m_channelList;
    std::vector<QPair<ChannelMonitorComponent*, libm2k::analog::DMM*>> m_activeChannels;
    QTimer* m_timer;
	scopy::gui::GenericMenu* m_generalSettingsMenu;

    std::vector<libm2k::analog::DMM*> getDmmList(libm2k::context::M2k* m2k_context);



	MonitorContainer* m_fixedColGrid;
    PlotContainer* plotContainer;
	QColor getChannelColor(int chId);
	QColor generateColor();

	QMap<int,QColor> m_color;
	QList<QColor> m_colors;

	scopy::gui::GenericMenu* generateMenu(QString title, QColor* color);

	void createConnections(scopy::gui::IIOMonitorGenericMenu* menu,adiscope::ChannelMonitorComponent* monitor);

	/////multitab
	scopy::gui::ChannelManager* m_monitorChannelManager;
	scopy::gui::ToolView* m_monitorToolView;
	QMap<int,ChannelMonitorComponent*> m_activeMonitors;
	void initMonitorToolView();

	scopy::gui::ChannelManager* m_plotChannelManager;
	scopy::gui::ToolView* m_plotToolView;
	QMap<PlotContainer*,QVector<int>> m_activePlots;
	scopy::gui::ToolView* generatePlotToolView();

	QMap<int,activeChannel> activeChannels;


    // /////TO BE REOMVED/////
	std::vector<double> testScale;
    int i;

    // ///////////////////

Q_SIGNALS:
	void PrecisionChanged(int precision);
	void RecordingIntervalChanged(double recordingInterval);

public Q_SLOTS:
    void readChannelValues();
    void toggleTimer(bool enabled);

    // /////TO BE REOMVED/////
    void testScaleFct();
};
}

#endif // IIOMONITOR_HPP
