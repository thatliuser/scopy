#include "dmmlist.hpp"
#include <QListWidgetItem>
#include <QSignalMapper>
#include <QLabel>
#include <QCheckBox>


/* libm2k includes */
#include <libm2k/contextbuilder.hpp>
#include <scopyExceptionHandler.h>
#include <libm2k/m2kexceptions.hpp>

using namespace adiscope;



DMMList::DMMList(struct iio_context *ctx, Filter *filt,
                 ToolMenuItem *toolMenuItem,
                 QJSEngine *engine, ToolLauncher *parent):

Tool(ctx, toolMenuItem, nullptr, "DMMList",
    parent),

  ui(new Ui::DMMList),
  m_m2k_context(m2kOpen(ctx, ""))
{
  ui->setupUi(this);
  run_button=nullptr;


    /*

     auto dmmList = m_m2k_context->getAllDmm();

     QSignalMapper* mapper = new QSignalMapper(this);
     int i =0;

     for(libm2k::analog::DMM* dmm : dmmList){


        //std::vector<DMM_READING> readings = dmm->readAll();
        //QCheckBox *dmmItem = new QCheckBox(QString::fromStdString(dmm->getName()),this);


        connect(dmmItem, SIGNAL(clicked(bool)), mapper, SLOT(map()));
        mapper->setMapping(dmmItem, i);
        mapper->setMapping(dmmItem, dmmItem->isChecked());

       // i++;
        ui->gridLayout->addWidget(dmmItem);
     }
     connect(mapper, SIGNAL(mapped(int)), this, SLOT(toggled(int)));
*/




}
void DMMList::setListOfDMMs(){

}
void DMMList::toggled(int){

}
void DMMList::addDmmInfo(int dmmIndex, bool state){

}

DMMList::~DMMList()
{
        delete ui;
}
