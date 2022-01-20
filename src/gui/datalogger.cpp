#include "datalogger.hpp"


using namespace adiscope;

DataLogger::DataLogger(bool lastValue, bool average, bool all)

{
	dataLoggerAPI = new DataLoggerAPI();
	dataLoggerUI = new DataLoggerUI(lastValue,average,all);

	connect(dataLoggerUI, &DataLoggerUI::pathChanged, dataLoggerAPI, &DataLoggerAPI::setPath);
	connect(dataLoggerUI, &DataLoggerUI::timeIntervalChanged, dataLoggerAPI, &DataLoggerAPI::setTimerInterval);
	connect(dataLoggerUI, &DataLoggerUI::toggleDataLogger, this, &DataLogger::toggleDataLogger);

	dataLoggerAPI->setTimerInterval(5000);
}

void DataLogger::startLogger()
{
  dataLoggerAPI->startLogger(dataLoggerUI->isOverwrite());
}

void DataLogger::stopLogger()
{
	dataLoggerAPI->stopLogger();
}

void DataLogger::createChannel(QString name, Type type)
{
	ch_Filter filter =ch_Filter::LAST_VALUE ;
	QString uiFilter = dataLoggerUI->getFilter();
	if(uiFilter == "Average"){
		filter = ch_Filter::AVERAGE;
	}
	if(uiFilter == "All"){
		filter = ch_Filter::ALL;
	}
	dataLoggerAPI->createChannel(name,type,filter);
}

void DataLogger::distroyChannel(QString name)
{
	dataLoggerAPI->distroyChannel(name);
}

void DataLogger::resetChannel(QString name)
{
	dataLoggerAPI->resetChannel(name);
}


void DataLogger::receiveValue(QString name, QString value)
{
	dataLoggerAPI->receiveValue(name,value);
}

QWidget* DataLogger::getWidget()
{
	return dataLoggerUI->getDataLoggerUIWidget();
}

bool DataLogger::isDataLoggerOn()
{
	return dataLoggerUI->isDataLoggingOn();
}

void DataLogger::dataLoggerToggled(bool toggled)
{
	Q_EMIT toggleDataLogger(toggled);
}

DataLogger::~DataLogger()
{
	delete dataLoggerAPI;
	delete dataLoggerUI;
}

