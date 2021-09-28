#include "iiomonitormenu.hpp"
#include "ui_iiomonitormenu.h"

using namespace scopy::gui;

IIOMonitorMenu::IIOMonitorMenu(QWidget *parent) :
    GenericMenu(parent),
    ui(new Ui::IIOMonitorMenu)
{
    ui->setupUi(this);

    showAllMenu(false);

    connect(ui->btnCollapsePeakHold, SIGNAL(toggled(bool)),this, SLOT(displayPeakHold(bool)));
    connect(ui->btnResetPeakHold, SIGNAL(clicked(bool)),this, SLOT(resetPeakHolderActivated(bool)));
    connect(ui->btnDisplayPeakHold, SIGNAL(toggled(bool)),this, SLOT(setPeakHolderToggled(bool)));
    connect(ui->histogramCh, SIGNAL(toggled(bool)), this, SLOT(setHistoryToggled(bool)));
    connect(ui->scaleBtn, SIGNAL(toggled(bool)), this, SLOT(setScaleToggled(bool)));
    connect(ui->showAllBtn, SIGNAL(toggled(bool)),this, SLOT(toggleAllWidgets(bool)));

}

void IIOMonitorMenu::displayPeakHold(bool checked)
{
    if(checked) {
        ui->peakHoledrControls->hide();
    }
    else  {
        ui->peakHoledrControls->show();
    }
}

void IIOMonitorMenu::setPeakHolderToggled(bool toggled){
    Q_EMIT IIOMonitorMenu::togglePeakHolder(toggled);
    ui->btnDisplayPeakHold->setChecked(toggled);
}

void IIOMonitorMenu::resetPeakHolderActivated(bool toggled){
    Q_EMIT IIOMonitorMenu::resetPeakHolder(toggled);
}

void IIOMonitorMenu::setHistoryToggled(bool toggled){
    Q_EMIT IIOMonitorMenu::toggleHistory(toggled);
    ui->btnResetPeakHold->setChecked(toggled);
}

void IIOMonitorMenu::setScaleToggled(bool toggled){
    Q_EMIT IIOMonitorMenu::toggleScale(toggled);
    ui->scaleBtn->setChecked(toggled);
}

void IIOMonitorMenu::toggleAllWidgets(bool toggled){
    Q_EMIT IIOMonitorMenu::toggleAll(toggled);
}

void IIOMonitorMenu::showAllMenu(bool enable){
    if(enable){
        ui->showAllBtn->show();
        ui->showAllBtn->setChecked(false);
        ui->label_7->show();
    }else{
        ui->showAllBtn->hide();
        ui->label_7->hide();
    }
}

IIOMonitorMenu::~IIOMonitorMenu()
{
    delete ui;
}
