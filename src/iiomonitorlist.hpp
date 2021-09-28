#ifndef IIOMONITORLIST_HPP
#define IIOMONITORLIST_HPP

#include "filter.hpp"
#include "tool_launcher.hpp"
#include "ui_iiomonitorlist.h"
#include "tool.hpp"
#include "gui/tool_view_builder.hpp"
#include "lcdNumber.hpp"
#include "ioomonitor.hpp"
#include <QPair>
#include "gui/channelmonitorcomponent.hpp"



/* libm2k includes */
#include <libm2k/analog/m2kanalogin.hpp>
#include <libm2k/m2k.hpp>
#include <libm2k/analog/dmm.hpp>

namespace adiscope {
class IIOMonitorList: public Tool
{
    Q_OBJECT
    Ui::IIOMonitorList *ui;
public:
    IIOMonitorList(struct iio_context *ctx, Filter *filt,
              ToolMenuItem *toolMenuItem,
              QJSEngine *engine, ToolLauncher *parent);
    ~IIOMonitorList();

     scopy::gui::ToolView* getToolView();

private:
    libm2k::context::M2k* m_m2k_context;
    scopy::gui::ToolView* m_toolView;
    scopy::gui::ChannelManager* m_channelManager;
    std::vector<libm2k::analog::DMM*> m_dmmList;
    std::vector<ChannelWidget*> m_channelList;
    std::vector<QPair<ChannelMonitorComponent*, libm2k::analog::DMM*>> m_activeChannels;

    std::vector<double> testScale;
    int i;


    void initChannels();

    QTimer* m_timer;

    IooMonitor iioMonitor;

public Q_SLOTS:
    void readChannelValues();
    void toggleTimer(bool enabled);

    void testScaleFct();

};
}

#endif // IIOMONITORLIST_HPP
