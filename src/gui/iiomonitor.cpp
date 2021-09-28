#include "iiomonitor.hpp"
#include "iiomonitorgeneralsettingsmenu.hpp"

#include "gui/generic_menu.hpp"

using namespace adiscope;

IIOMonitor::IIOMonitor(struct iio_context *ctx, Filter *filt,
    ToolMenuItem *toolMenuItem,
    QJSEngine *engine, ToolLauncher *parent):
    Tool(ctx, toolMenuItem, nullptr, "IIOMonitor",
         parent),
    ui(new Ui::IIOMonitor),
    m_m2k_context(m2kOpen(ctx, "")),
    m_timer(new QTimer(this)),
    m_generalSettingsMenu (new scopy::gui::IIOMonitorGeneralSettingsMenu())

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


    //m_channelManager->getAddChannelBtn()->setVisible(true);

    m_channelManager = new scopy::gui::ChannelManager(recepie.channelsPosition);

    m_toolView = scopy::gui::ToolViewBuilder(recepie,m_channelManager).build();




    m_toolView->setGeneralSettingsMenu(m_generalSettingsMenu,false);
  //  m_toolView->getGeneralSettingsBtn()->hide();

    m_dmmList=getDmmList(m_m2k_context);

    initChannels();

    connect(m_toolView->getRunBtn(), SIGNAL(toggled(bool)), this, SLOT(toggleTimer(bool)));
    connect(m_toolView->getRunBtn(), SIGNAL(toggled(bool)),runButton(), SLOT(setChecked(bool)));
    connect(runButton(), SIGNAL(toggled(bool)), m_toolView->getRunBtn(),SLOT(setChecked(bool)));
    connect(m_toolView->getRunBtn(), SIGNAL(toggled(bool)),this, SLOT(startDataLogging(bool)));

     // //////////////////////////////// TO BE REMOVED ////
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

   //  m_toolView->buildNewInstrumentMenu(new scopy::gui::GenericMenu,true,"Ioana",false,false);



     // ////////////////////////////////////////////
     connect(m_toolView->getSingleBtn(), SIGNAL(toggled(bool)),this, SLOT(readChannelValues()));

    connect(m_toolView->getSingleBtn(), &QPushButton::clicked,this, [=](bool checked){
            m_toolView->getSingleBtn()->setChecked(!checked);
     });

    connect(m_generalSettingsMenu, &scopy::gui::IIOMonitorGeneralSettingsMenu::recordingTimeChanged,this, [=](double value){

        VALUE_READING_TIME_INTERVAL = value;
        toggleTimer(false);
        toggleTimer(true);
    });
}


void IIOMonitor::initChannels(){

   int chId = 0;
   MonitorContainer* monitorContainer = new MonitorContainer(this);
   monitorContainer->setMaxColumnNumber(2);
   for(libm2k::analog::DMM* dmm : m_dmmList){

       scopy::gui::IIOMonitorMenu* menu = new scopy::gui::IIOMonitorMenu(this);
       menu->showAllMenu(true);

       ChannelWidget *ch_widget =
               m_toolView->buildNewChannel(m_channelManager, menu, true, chId , false, false, QColor("green"), "Channel", QString::fromStdString(dmm->getName()) );

       ch_widget->enableButton()->setChecked(false);

       std::vector<ChannelWidget*> channelList;

       for(auto channel : dmm->readAll()){

         scopy::gui::IIOMonitorMenu* m = new scopy::gui::IIOMonitorMenu(this);

         ChannelWidget *ch_widget =
                 m_toolView->buildNewChannel(m_channelManager, m, true, chId , false, false, QColor("#ff7200"), "Channel", QString::fromStdString(channel.id) );

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

       connect(m_generalSettingsMenu, &scopy::gui::IIOMonitorGeneralSettingsMenu::precisionChanged, this, [=](int precision){
            monitor->updateLcdNumberPrecision(precision);
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

      // logic for adding/removing channels
       connect(ch_widget, &ChannelWidget::enabled,this, [=](){
              if(!monitorContainer->isWidgetActive(widgetId)){
                      monitorContainer->addWidget(widgetId);
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

       //make channel read value after timeout
        connect(m_timer, &QTimer::timeout , this, &IIOMonitor::readChannelValues);
          chId++;
         }

       chId++;
       m_toolView->buildChannelGroup(ch_widget,channelList);

     }

     m_channelManager->insertAddBtn(new scopy::gui::GenericMenu,true);
    m_toolView->addDockableCentralWidget(monitorContainer,Qt::TopDockWidgetArea, QString::fromStdString(""));
}

scopy::gui::ToolView* IIOMonitor::getToolView(){
    return m_toolView;
}

std::vector<libm2k::analog::DMM*> IIOMonitor::getDmmList(libm2k::context::M2k* m2k_context){
    return m2k_context->getAllDmm() ;
}

void IIOMonitor::readChannelValues(){
    for(auto ch : m_activeChannels){
        auto aux = ch.second->readChannel(ch.first->getChannelId());
        ch.first->updateValue(aux.value, QString::fromStdString(aux.unit));
    }
}


void IIOMonitor::toggleTimer(bool enabled){
    readChannelValues();
    // set timer for 5s
    if(enabled){
        m_timer->start(VALUE_READING_TIME_INTERVAL);
    }else{
        m_timer->stop();
    }
}

void IIOMonitor::testScaleFct(){
    auto aux = m_activeChannels.at(0).second->readChannel(m_activeChannels.at(0).first->getChannelId());
    m_activeChannels.at(0).first->updateValue(testScale.at(i), QString::fromStdString(aux.unit));

    if ( i < testScale.size() - 1 ){
        i++;
    }else{
        i = 0;
    }
}


IIOMonitor::~IIOMonitor()
{
    if(m_m2k_context){ delete m_m2k_context; }
    if(m_toolView){ delete m_toolView; }
    if(m_channelManager){ delete m_channelManager; }
    if(m_generalSettingsMenu){delete m_generalSettingsMenu;}

    for(auto dmm : m_dmmList){
        delete dmm;
    }
    m_dmmList.clear();

    for(auto channel : m_channelList){
        delete channel;
    }
    m_channelList.clear();

    for(auto activeCh : m_activeChannels){
        delete activeCh.first;
        delete activeCh.second;
    }

    delete ui;
}
