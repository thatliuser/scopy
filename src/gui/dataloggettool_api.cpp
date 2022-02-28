#include "dataloggettool_api.h"

namespace adiscope{

bool DataLoggetTool_api::get_show_all()
{
	return dataLoggerTool->showAllSWitch->isChecked();
}

void DataLoggetTool_api::set_show_all(bool en)
{
	dataLoggerTool->showAllSWitch->setChecked(en);
}

int DataLoggetTool_api::get_precision()
{
	return dataLoggerTool->getPrecision();
}

void DataLoggetTool_api::set_precision(int precision)
{
	 dataLoggerTool->setPrecision(precision);
	 Q_EMIT dataLoggerTool->PrecisionChanged(precision);
}

int DataLoggetTool_api::get_recording_interval()
{
	return dataLoggerTool->getValueReadingTimeInterval();
}

void DataLoggetTool_api::set_recording_interval(int interval)
{
	Q_EMIT dataLoggerTool->RecordingIntervalChanged(interval);
}

bool DataLoggetTool_api::get_data_logging_on(){
	return dataLoggerTool->dataLogger->isDataLoggerOn();
}
void DataLoggetTool_api::set_data_logging_on(bool en){
	dataLoggerTool->dataLogger->dataLoggerToggled(en);
}

QString DataLoggetTool_api::get_data_logging_path()
{
	return dataLoggerTool->dataLogger->getPath();
}

void DataLoggetTool_api::set_data_logging_path(QString path)
{
	if(dataLoggerTool->dataLogger->isDataLoggerOn()){
		dataLoggerTool->dataLogger->setPath(path);
	}else{
		dataLoggerTool->dataLogger->setPath("");
	}
}

bool DataLoggetTool_api::get_data_logger_overwrite_append()
{
	return dataLoggerTool->dataLogger->isOverwrite();
}

void DataLoggetTool_api::set_data_logger_overwrite_append(bool en)
{
	if(dataLoggerTool->dataLogger->isDataLoggerOn()){
		dataLoggerTool->dataLogger->setOverwrite(en);
	}
}

int DataLoggetTool_api::get_data_logger_recording_interval()
{
	return dataLoggerTool->dataLogger->getRecordingTimeInterval();
}

void DataLoggetTool_api::set_data_logger_recording_interval(int interval)
{
	if(dataLoggerTool->dataLogger->isDataLoggerOn()){
		dataLoggerTool->dataLogger->setRecodingTimeInterval(interval);
	}
}

}
