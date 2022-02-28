#ifndef DATALOGGERUI_HPP
#define DATALOGGERUI_HPP

#include <QWidget>
#include <QLineEdit>
#include <QRadioButton>
#include "customSwitch.hpp"
#include <QVBoxLayout>
#include "spinbox_a.hpp"
#include <QFileDialog>
#include <qcombobox.h>

namespace adiscope {
class DataLoggerUI : public QWidget
{
	Q_OBJECT

public:
	explicit DataLoggerUI(bool lastValue, bool average, bool all, QWidget *parent = nullptr);
	~DataLoggerUI();
	//UI
	PositionSpinButton *data_logging_timer;
	QWidget *dataLoggingWidget;
	QVBoxLayout *dataLoggingLayout;
	QLineEdit *dataLoggingFilePath;
	QRadioButton *overwriteRadio;
	QRadioButton *appendRadio;
	CustomSwitch *dataLoggingSwitch;
	QComboBox *dataLoggerFilter;
	QString filename;
	//functions
	void init();
	void chooseFile();
	QWidget* getDataLoggerUIWidget();
	bool isDataLoggingOn();
	bool isOverwrite();
	QString getFilter();

Q_SIGNALS:
	void toggleDataLogger(bool toggled);
	void pathChanged(QString path);
	void timeIntervalChanged(double inetrval);

};
}
#endif // DATALOGGERUI_HPP
