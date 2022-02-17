#ifndef DATALOGGETTOOL_API_H
#define DATALOGGETTOOL_API_H

#include <QObject>
#include "apiObject.hpp"
#include "iiomonitor.hpp"

namespace adiscope {
class DataLoggetTool_api:public ApiObject
{
	Q_OBJECT

public:
	DataLoggetTool_api(DataLoggerTool *dataLoggerTool): ApiObject(), dataLoggerTool(dataLoggerTool) {};

private:
	DataLoggerTool *dataLoggerTool;
};
}
#endif // DATALOGGETTOOL_API_H
