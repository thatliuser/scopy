#ifndef IIOMONITORGENERALSETTINGSMENU_HPP
#define IIOMONITORGENERALSETTINGSMENU_HPP

#include <QWidget>
#include "gui/generic_menu.hpp"
#include "spinbox_a.hpp"

namespace Ui {
class IIOMonitorGeneralSettingsMenu;
}
namespace scopy {
namespace gui {
class IIOMonitorGeneralSettingsMenu : public GenericMenu
{
    Q_OBJECT

public:
    explicit IIOMonitorGeneralSettingsMenu(QWidget *parent = nullptr);
    ~IIOMonitorGeneralSettingsMenu();

    void setPrecision(int digitCount);
    int getPrecision();

private:
    Ui::IIOMonitorGeneralSettingsMenu *ui;
    PositionSpinButton *recording_timer;

    int m_precision;

Q_SIGNALS:
    void precisionChanged(int precision);
    void recordingTimeChanged(double recordingTime);
};
}
}
#endif // IIOMONITORGENERALSETTINGSMENU_HPP
