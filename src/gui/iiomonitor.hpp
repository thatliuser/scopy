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
#include <QRadioButton>
#include "gui/channelmonitorcomponent.hpp"
#include "gui/iiomonitormenu.hpp"
#include "gui/monitorcontainer.hpp"
#include "iiomonitorgeneralsettingsmenu.hpp"

#include "gui/iiomonitorgenericmenu.hpp"
#include <QFileDialog>

#include "datalogger.hpp"

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

	QTimer* m_timer;
	DataLogger *dataLogger;
	QList<QColor> m_colors;
	QMap<int,QColor> m_color;
	scopy::gui::ToolView* m_toolView;
	MonitorContainer* m_fixedColGrid;
	libm2k::context::M2k* m_m2k_context;
	QMap<int,activeChannel> activeChannels;
	scopy::gui::ToolView* m_monitorToolView;
	std::vector<ChannelWidget*> m_channelList;
	std::vector<libm2k::analog::DMM*> m_dmmList;
	scopy::gui::GenericMenu* m_generalSettingsMenu;
	scopy::gui::ChannelManager* m_monitorChannelManager;
	QMap<int,ChannelMonitorComponent*> m_activeMonitors;
    std::vector<QPair<ChannelMonitorComponent*, libm2k::analog::DMM*>> m_activeChannels;

	QColor generateColor();
	void initMonitorToolView();
	QColor getChannelColor(int chId);
	libm2k::analog::DMM_READING readChVal(int ch);
	scopy::gui::GenericMenu* generateMenu(QString title, QColor* color);
	std::vector<libm2k::analog::DMM*> getDmmList(libm2k::context::M2k* m2k_context);
	void createConnections(scopy::gui::IIOMonitorGenericMenu* menu,adiscope::ChannelMonitorComponent* monitor);

    // /////TO BE REOMVED/////
	std::vector<double> testScale;
    int i;
    // ///////////////////

Q_SIGNALS:
	void PrecisionChanged(int precision);
	void updateValue(QString name, QString value);
	void RecordingIntervalChanged(double recordingInterval);

public Q_SLOTS:
    void readChannelValues();
    void toggleTimer(bool enabled);

    // /////TO BE REOMVED/////
    void testScaleFct();
};
}
#endif // IIOMONITOR_HPP
