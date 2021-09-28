#ifndef IIOMONITORMENU_HPP
#define IIOMONITORMENU_HPP

#include <QWidget>
#include "gui/generic_menu.hpp"

namespace Ui {
class IIOMonitorMenu;
}
namespace scopy {
namespace gui {
class IIOMonitorMenu : public GenericMenu
{
    Q_OBJECT

public:
    explicit IIOMonitorMenu(QWidget *parent = nullptr);
    ~IIOMonitorMenu();

    void showAllMenu(bool enable);

Q_SIGNALS:
    void togglePeakHolder(bool toggled);
    void resetPeakHolder(bool toggled);
    void toggleHistory(bool toggled);
    void toggleScale(bool toggle);
    void toggleAll(bool toggle);


public Q_SLOTS:
    void setPeakHolderToggled(bool toggled);
    void displayPeakHold(bool display);
    void setHistoryToggled(bool toggled);
    void setScaleToggled(bool toggled);
    void resetPeakHolderActivated(bool toggled);
    void toggleAllWidgets(bool toggled);



private:
    Ui::IIOMonitorMenu *ui;
};
} // namespace gui
} // namespace scopy


#endif // IIOMONITORMENU_HPP
