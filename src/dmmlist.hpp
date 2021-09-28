#ifndef DMMLIST_HPP
#define DMMLIST_HPP

#include "filter.hpp"
#include "tool_launcher.hpp"
#include "ui_dmmlist.h"
#include "tool.hpp"
#include "gui/tool_view_builder.hpp"

/* libm2k includes */
#include <libm2k/analog/m2kanalogin.hpp>
#include <libm2k/m2k.hpp>

namespace adiscope {
class DMMList: public Tool
{
    Ui::DMMList *ui;
public:
    DMMList(struct iio_context *ctx, Filter *filt,
              ToolMenuItem *toolMenuItem,
              QJSEngine *engine, ToolLauncher *parent);
    ~DMMList();



private:
    libm2k::context::M2k* m_m2k_context;
    std::vector<libm2k::analog::DMM*> m_dmmList;


  //  std::vector<QCheckBox*> dmms;
    void setListOfDMMs();
    void toggled(int);
    void addDmmInfo(int dmmIndex, bool state);

};
}

#endif // DMMLIST_HPP
