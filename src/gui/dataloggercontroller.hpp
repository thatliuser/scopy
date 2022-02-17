#ifndef DATALOGGERCONTROLLER_HPP
#define DATALOGGERCONTROLLER_HPP

#include <QObject>

namespace adiscope {
class DataLoggerController: public QObject
{
	Q_OBJECT
public:
	DataLoggerController();

	//DataLogger *dataLogger;

Q_SIGNALS:
	void setPath(QString path);
	void setTimeInterval(double interval);
	void start(bool overwrite);
	void stop();


};
}
#endif // DATALOGGERCONTROLLER_HPP
