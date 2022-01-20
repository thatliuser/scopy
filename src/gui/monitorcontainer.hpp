#ifndef MONITORCONTAINER_HPP
#define MONITORCONTAINER_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QResizeEvent>

namespace Ui {
class MonitorContainer;
}

class MonitorContainer : public QWidget
{
	Q_OBJECT

public:
	explicit MonitorContainer(int maxCols, QWidget *parent = nullptr);
	~MonitorContainer();

	void toggleAll(bool toggled);
	void addWidget(int index);
	void removeWidget(int index);
	QWidget* getWidget(int index);
	bool isWidgetActive(int index);
	int addQWidgetToList(QWidget *widget);

	void setMaxColumnNumber(int maxColumns);
	int getMaxColumnNumber();

private:

	int m_maxCols;
	int currentNumberOfCols;
	int col;
	int row;
	std::vector<QWidget*> m_widgetList;
	QVector<int> m_activeWidgetList;
	QGridLayout *m_gridLayout;
	QWidget *m_mainWidget;
	QSpacerItem *m_hspacer;
	QSpacerItem *m_vspacer;

	Ui::MonitorContainer *ui;

	void repositionWidgets(int index, int row, int col);
	void redrawWidgets();
	void resizeEvent(QResizeEvent *event);
};

#endif // MONITORCONTAINER_HPP
