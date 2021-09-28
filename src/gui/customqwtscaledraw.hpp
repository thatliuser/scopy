#ifndef CUSTOMQWTSCALEDRAW_HPP
#define CUSTOMQWTSCALEDRAW_HPP

#include <qwt_scale_draw.h>
#include "plot_utils.hpp"
#include <qwt_text.h>
#include "QwtLinearScaleEngine"
#include "QwtInterval"
#include "QwtText"

namespace adiscope {

class CustomQwtScaleDraw : public QwtScaleDraw
{
public:
    CustomQwtScaleDraw();

protected:
   virtual QwtText label(double value) const;

};
}
#endif // CUSTOMQWTSCALEDRAW_HPP
