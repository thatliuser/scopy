#ifndef IIOMONITORGENERICMENU_HPP
#define IIOMONITORGENERICMENU_HPP

#include <QWidget>
#include "gui/generic_menu.hpp"
#include "spinbox_a.hpp"
#include "qcombobox.h"
#include "customSwitch.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace scopy {
namespace gui {
class IIOMonitorGenericMenu : public GenericMenu
{
	Q_OBJECT
public:
	explicit IIOMonitorGenericMenu(QWidget *parent = nullptr);
	~IIOMonitorGenericMenu();

	void init(QString title, QColor* color,bool showAll);

Q_SIGNALS:
	void toggleScale(bool toggle);
	void monitorColorChanged(QString color);
	void toggleHistory(bool toggled);
	void changeHistorySize(int size);
	void lineStyleChanged(Qt::PenStyle lineStyle);
	void resetPeakHolder();
	void togglePeakHolder(bool toggled);
	void toggleAll(bool toggle);

private:
	double numSamplesFromIdx(int idx);
	Qt::PenStyle lineStyleFromIdx(int idx);
	void setHistorySize(int idx);
};
}
}

#endif // IIOMONITORGENERICMENU_HPP
