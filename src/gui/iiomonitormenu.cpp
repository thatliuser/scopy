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
    connect(ui->historySize, SIGNAL(currentIndexChanged(int)), this, SLOT(setHistorySize(int)));
    connect(ui->btnCollapseHistory, SIGNAL(toggled(bool)), this, SLOT(displayHistory(bool)));

    connect(ui->setColorBtn, &QPushButton::clicked,this, [=](){
       Q_EMIT monitorColorChanged(ui->colorValue->text());
    });

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


void IIOMonitorMenu::showChangeColor(bool enabled){
    if(enabled){
        ui->setColorBtn->show();
        ui->colorValue->show();
    }else{
        ui->setColorBtn->hide();
        ui->colorValue->hide();
    }
}

void IIOMonitorMenu::setHistorySize(int idx)
{
    Q_EMIT changeHistorySize(numSamplesFromIdx(idx));
}

int IIOMonitorMenu::numSamplesFromIdx(int idx)
{
    switch(idx) {
    case 0:
        return 10;
    case 1:
        return 100;
    case 2:
        return 600;
    default:
        throw std::runtime_error("Invalid IDX");
    }
}

void IIOMonitorMenu::displayHistory(bool toggled){
    if(toggled){
        ui->histogramCh->hide();
        ui->historySize->hide();
    }else{
         ui->histogramCh->show();
         ui->historySize->show();
    }

}

IIOMonitorMenu::~IIOMonitorMenu()
{
    delete ui;
}
