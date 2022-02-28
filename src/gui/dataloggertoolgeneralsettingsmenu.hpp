#ifndef DATALOGGERTOOLGENERALSETTINGSMENU_HPP
#define DATALOGGERTOOLGENERALSETTINGSMENU_HPP

#include <QWidget>
#include "gui/generic_menu.hpp"
#include "spinbox_a.hpp"

namespace Ui {
class DataLoggerToolGeneralSettingsMenu;
}
namespace scopy {
namespace gui {
class DataLoggerToolGeneralSettingsMenu : public GenericMenu
{
    Q_OBJECT

public:
	explicit DataLoggerToolGeneralSettingsMenu(QWidget *parent = nullptr);
	~DataLoggerToolGeneralSettingsMenu();

    void setPrecision(int digitCount);
    int getPrecision();

private:
	Ui::DataLoggerToolGeneralSettingsMenu *ui;
    PositionSpinButton *recording_timer;

    int m_precision;

Q_SIGNALS:
    void precisionChanged(int precision);
    void recordingTimeChanged(double recordingTime);
};
}
}
#endif // DATALOGGERTOOLGENERALSETTINGSMENU_HPP
