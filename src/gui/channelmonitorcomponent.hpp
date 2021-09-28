#ifndef CHANNELMONITORCOMPONENT_H
#define CHANNELMONITORCOMPONENT_H

#include <QWidget>

namespace Ui {
class ChannelMonitorComponent;
}

namespace adiscope {
class ChannelMonitorComponent : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelMonitorComponent(QWidget *parent = nullptr);
    ~ChannelMonitorComponent();

    void init(double value, QString unitOfMeasure, QString title);
    std::string getChannelId();
    void setChannelId(std::string channelId);
    void checkPeakValues(double value, QString unitOfMeasure);
    void updateUnitOfMeasure(QString unitOfMeasure);
    QString getUnitOfMeasure();

public Q_SLOTS:
    void displayPeakHold(bool checked);
    void resetPeakHolder();
    void displayHistory(bool checked);
    void displayScale(bool checked);
    void updateLcdNumberPrecision(int precision);

public Q_SIGNAL:
    void updateValue(double value, QString unitOfMeasure);

private:
    Ui::ChannelMonitorComponent *ui;
    std::string m_channelId;
    double m_minValue;
    double m_maxValue;
    QString m_unitOfMeasure;
};

}

#endif // CHANNELMONITORCOMPONENT_H
