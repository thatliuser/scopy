#include "iiomonitorlist.hpp"
#include <QListWidgetItem>
#include <QSignalMapper>
#include <QLabel>
#include <QCheckBox>
#include "gui/tool_view_builder.hpp"
#include "gui/tool_view.hpp"
#include "gui/iiomonitormenu.hpp"
#include "gui/monitorcontainer.hpp"
#include <QSignalMapper>

/* libm2k includes */
#include <libm2k/contextbuilder.hpp>
#include <scopyExceptionHandler.h>
#include <libm2k/m2kexceptions.hpp>



using namespace adiscope;



IIOMonitorList::IIOMonitorList(struct iio_context *ctx, Filter *filt,
                 ToolMenuItem *toolMenuItem,
                 QJSEngine *engine, ToolLauncher *parent):

Tool(ctx, toolMenuItem, nullptr, "IIOMonitorList",
    parent),

  ui(new Ui::IIOMonitorList),
  m_m2k_context(m2kOpen(ctx, "")),
  m_timer(new QTimer(this))
{
  ui->setupUi(this);
  run_button=nullptr;



  scopy::gui::ToolViewRecipe recepie;
  recepie.helpBtnUrl="";
  recepie.hasRunBtn = true;
  recepie.hasSingleBtn = true;
  recepie.hasPairSettingsBtn = true;
  recepie.hasPrintBtn = true;
  recepie.hasChannels = true;
  recepie.channelsPosition = scopy::gui::ChannelsPositionEnum::VERTICAL;

  m_channelManager = new scopy::gui::ChannelManager(recepie.channelsPosition);

  m_toolView = scopy::gui::ToolViewBuilder(recepie,m_channelManager).build();

  m_dmmList=iioMonitor.getDmmList(m_m2k_context);


 initChannels();

// connect(m_toolView->getSingleBtn(),SIGNAL(toggled(bool)), this, SLOT(readChannelValues(bool)));

 connect(m_toolView->getRunBtn(), SIGNAL(toggled(bool)), this, SLOT(toggleTimer(bool)));

 testScale.push_back(0.00000000005);
 testScale.push_back(0.000000005);
 testScale.push_back(0.000005);
 testScale.push_back(0.005);
 testScale.push_back(0.05);
 testScale.push_back(0.5);
 testScale.push_back(5);
 testScale.push_back(50);
 testScale.push_back(500);
 testScale.push_back(5000);
 testScale.push_back(5000000);
 testScale.push_back(5000000000);
 i = 0;

 connect(m_toolView->getSingleBtn(), SIGNAL(toggled(bool)),this, SLOT(testScaleFct()));

}

scopy::gui::ToolView* IIOMonitorList::getToolView(){
    return m_toolView;
}

void IIOMonitorList::initChannels(){

   int chId = 0;
   MonitorContainer* monitorContainer = new MonitorContainer(this);
   for(libm2k::analog::DMM* dmm : m_dmmList){

       scopy::gui::IIOMonitorMenu* menu = new scopy::gui::IIOMonitorMenu(this);
       menu->showAllMenu(true);

       ChannelWidget *ch_widget =
               m_toolView->buildNewChannel(m_channelManager, menu, true, chId , false, false, QColor("green"), "Channel", QString::fromStdString(dmm->getName()) );

       ch_widget->enableButton()->setChecked(false);

       std::vector<ChannelWidget*> channelList;



       for(libm2k::analog::DMM_READING channel : dmm->readAll()){



         scopy::gui::IIOMonitorMenu* m = new scopy::gui::IIOMonitorMenu(this);

         ChannelWidget *ch_widget =
                 m_toolView->buildNewChannel(m_channelManager, m, true, chId , false, false, QColor("blue"), "Channel", QString::fromStdString(channel.id) );

       channelList.push_back(ch_widget);

       ch_widget->enableButton()->setChecked(false);
       ch_widget->hide();

       adiscope::ChannelMonitorComponent* monitor = new adiscope::ChannelMonitorComponent();

       monitor->init(0,QString::fromStdString(channel.unit),QString::fromStdString(dmm->getName() + ": " + channel.id));
       //channel.value,QString::fromStdString(channel.unit), QString::fromStdString(dmm->getName() + ": " + channel.id));
       monitor->setChannelId(channel.id);


       //connect widget to group settings (main menu)

       connect(menu, &scopy::gui::IIOMonitorMenu::toggleAll, this, [=](bool toggled){
           if(ch_widget->enableButton()->isChecked() != toggled){
            ch_widget->enableButton()->click();
           }
       });


       connect(menu, &scopy::gui::IIOMonitorMenu::togglePeakHolder,this,[=](bool toggled){
           m->setPeakHolderToggled(toggled);

       });

       connect(menu, &scopy::gui::IIOMonitorMenu::toggleHistory,this,[=](bool toggled){
           m->setHistoryToggled(toggled);

       });

       connect(menu, &scopy::gui::IIOMonitorMenu::toggleScale,this,[=](bool toggled){
           m->setScaleToggled(toggled);

       });

       connect(menu, &scopy::gui::IIOMonitorMenu::resetPeakHolder,this,[=](bool toggled){
           m->resetPeakHolderActivated(toggled);

       });



       //connect menu switches to widget

       connect(m,&scopy::gui::IIOMonitorMenu::togglePeakHolder,this,[=](bool toggled){
          monitor->displayPeakHold(toggled);
       });

       connect(m, &scopy::gui::IIOMonitorMenu::toggleHistory,this,[=](bool toggled){
           monitor->displayHistory(toggled);
       });

       connect(m, &scopy::gui::IIOMonitorMenu::toggleScale,this, [=](bool toggled){
          monitor->displayScale(toggled);
       });

       connect(m, &scopy::gui::IIOMonitorMenu::resetPeakHolder, this, [=](){
          monitor->resetPeakHolder();
       });


      int widgetId =monitorContainer->addMonitorContainerToList(monitor);

       connect(ch_widget, &ChannelWidget::enabled,this, [=](){

              if(!monitorContainer->isWidgetActive(widgetId)){
                      monitorContainer->addWidget(widgetId);
                   //   monitor->init(channel.value,QString::fromStdString(channel.unit),QString::fromStdString(channel.id));
                      m_activeChannels.push_back(qMakePair(monitor,dmm));
                  }else{
                      monitorContainer->removeWidget(widgetId);
                      for(int index = 0; index < m_activeChannels.size(); index++){
                          if(m_activeChannels.at(index).first == monitor){
                              m_activeChannels.erase(m_activeChannels.begin() + index);
                              break;
                          }
                      }
                  }
          });

          connect(m_timer, &QTimer::timeout , this, &IIOMonitorList::readChannelValues);
          chId++;
         }

        chId++;
       m_toolView->buildChannelGroup(ch_widget,channelList);
     }
    m_toolView->addDockableCentralWidget(monitorContainer,Qt::TopDockWidgetArea, QString::fromStdString(""));
}

void IIOMonitorList::readChannelValues(){
    for(auto ch : m_activeChannels){
        auto aux = ch.second->readChannel(ch.first->getChannelId());
        ch.first->updateValue(aux.value, QString::fromStdString(aux.unit));
    }
}

void IIOMonitorList::toggleTimer(bool enabled){
    Q_EMIT readChannelValues();
    if(enabled){
        m_timer->start(5000);
    }else{
        m_timer->stop();
    }
}

void IIOMonitorList::testScaleFct(){


    auto aux = m_activeChannels.at(0).second->readChannel(m_activeChannels.at(0).first->getChannelId());
    m_activeChannels.at(0).first->updateValue(testScale.at(i), QString::fromStdString(aux.unit));

   // auto aux2 = m_activeChannels.at(1).second->readChannel(m_activeChannels.at(1).first->getChannelId());
   // m_activeChannels.at(1).first->updateValue(testScale.at(i+1), QString::fromStdString(aux2.unit));

    if ( i < testScale.size() - 1 ){
        i++;
    }else{
        i = 0;
    }
}


IIOMonitorList::~IIOMonitorList()
{
        delete ui;

    if(m_m2k_context){ delete m_m2k_context; }
    if(m_toolView){ delete m_toolView; }
    if(m_channelManager){ delete m_channelManager; }
}
