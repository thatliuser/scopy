#include "dataloggertoolgeneralsettingsmenu.hpp"
#include "ui_dataloggertoolgeneralsettingsmenu.h"

using namespace scopy::gui;

DataLoggerToolGeneralSettingsMenu::DataLoggerToolGeneralSettingsMenu(QWidget *parent) :
    GenericMenu(parent),
	ui(new Ui::DataLoggerToolGeneralSettingsMenu)
{
    ui->setupUi(this);

    ui->precisionValue->setText("3");

    recording_timer = new PositionSpinButton({
        {"s", 1},
        {"min", 60},
        {"h", 3600}
    }, tr("Timer"), 0, 3600,
    true, false, this);

    recording_timer->setValue(5);

    ui->verticalLayout_3->addWidget(recording_timer);

    connect(recording_timer, &PositionSpinButton::valueChanged,this, [=](){
        Q_EMIT recordingTimeChanged(recording_timer->value() * 1000);
    });

    connect(ui->setPrecisionBtn, &QPushButton::clicked,this, [=](){
        setPrecision((ui->precisionValue->text()).toInt());
    });
}

void DataLoggerToolGeneralSettingsMenu::setPrecision(int precision){
    m_precision = precision;
    Q_EMIT precisionChanged(precision);
}

int DataLoggerToolGeneralSettingsMenu::getPrecision(){return m_precision;}

DataLoggerToolGeneralSettingsMenu::~DataLoggerToolGeneralSettingsMenu()
{
    delete ui;
}
