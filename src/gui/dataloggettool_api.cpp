#include "dataloggettool_api.h"

namespace adiscope{

bool DataLoggerTool_api::get_show_all()
{
	return dataLoggerTool->showAllSWitch->isChecked();
}

void DataLoggerTool_api::set_show_all(bool en)
{
	dataLoggerTool->showAllSWitch->setChecked(en);
}

int DataLoggerTool_api::get_precision()
{
	return dataLoggerTool->getPrecision();
}

void DataLoggerTool_api::set_precision(int precision)
{
	 dataLoggerTool->setPrecision(precision);
	 Q_EMIT dataLoggerTool->PrecisionChanged(precision);
}

int DataLoggerTool_api::get_recording_interval()
{
	return dataLoggerTool->getValueReadingTimeInterval();
}

void DataLoggerTool_api::set_recording_interval(int interval)
{
	Q_EMIT dataLoggerTool->RecordingIntervalChanged(interval);
}

bool DataLoggerTool_api::get_data_logging_on(){
	return dataLoggerTool->dataLogger->isDataLoggerOn();
}
void DataLoggerTool_api::set_data_logging_on(bool en){
	dataLoggerTool->dataLogger->dataLoggerToggled(en);
}

QString DataLoggerTool_api::get_data_logging_path()
{
	return dataLoggerTool->dataLogger->getPath();
}

void DataLoggerTool_api::set_data_logging_path(QString path)
{
	if(dataLoggerTool->dataLogger->isDataLoggerOn()){
		dataLoggerTool->dataLogger->setPath(path);
	}else{
		dataLoggerTool->dataLogger->setPath("");
	}
}

bool DataLoggerTool_api::get_data_logger_overwrite_append()
{
	return dataLoggerTool->dataLogger->isOverwrite();
}

void DataLoggerTool_api::set_data_logger_overwrite_append(bool en)
{
	if(dataLoggerTool->dataLogger->isDataLoggerOn()){
		dataLoggerTool->dataLogger->setOverwrite(en);
	}
}

int DataLoggerTool_api::get_data_logger_recording_interval()
{
	return dataLoggerTool->dataLogger->getRecordingTimeInterval();
}

void DataLoggerTool_api::set_data_logger_recording_interval(int interval)
{
	if(dataLoggerTool->dataLogger->isDataLoggerOn()){
		dataLoggerTool->dataLogger->setRecodingTimeInterval(interval);
	}
}

}
