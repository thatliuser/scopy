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

/* libm2k includes */
#include <libm2k/analog/m2kanalogin.hpp>
#include <libm2k/m2k.hpp>
#include <libm2k/analog/dmm.hpp>
#include <libm2k/contextbuilder.hpp>
#include <scopyExceptionHandler.h>
#include <libm2k/m2kexceptions.hpp>

namespace adiscope {

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
     int VALUE_READING_TIME_INTERVAL = 5000;

    libm2k::context::M2k* m_m2k_context;
    scopy::gui::ToolView* m_toolView;
    scopy::gui::ChannelManager* m_channelManager;
    std::vector<libm2k::analog::DMM*> m_dmmList;
    std::vector<ChannelWidget*> m_channelList;
    std::vector<QPair<ChannelMonitorComponent*, libm2k::analog::DMM*>> m_activeChannels;
    QTimer* m_timer;
    scopy::gui::IIOMonitorGeneralSettingsMenu* m_generalSettingsMenu;

    std::vector<libm2k::analog::DMM*> getDmmList(libm2k::context::M2k* m2k_context);

    void initChannels();

    PlotContainer* plotContainer;

    // /////TO BE REOMVED/////

    std::vector<double> testScale;
    int i;

    // ///////////////////

public Q_SLOTS:
    void readChannelValues();
    void toggleTimer(bool enabled);

    // /////TO BE REOMVED/////
    void testScaleFct();
};
}

#endif // IIOMONITOR_HPP
