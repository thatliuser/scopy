#include "iiomonitor.hpp"
#include "iiomonitorgeneralsettingsmenu.hpp"

#include "gui/generic_menu.hpp"
#include "gui/verticalcontrol.hpp"
#include "gui/customSwitch.hpp"
#include "dynamicWidget.hpp"


using namespace adiscope;

IIOMonitor::IIOMonitor(struct iio_context *ctx, Filter *filt,
    ToolMenuItem *toolMenuItem,
    QJSEngine *engine, ToolLauncher *parent):
    Tool(ctx, toolMenuItem, nullptr, "IIOMonitor",
         parent),
    ui(new Ui::IIOMonitor),
    m_m2k_context(m2kOpen(ctx, "")),
	m_timer(new QTimer(this))
{
	ui->setupUi(this);
	run_button = nullptr;

	scopy::gui::ToolViewRecipe recepie;
	recepie.helpBtnUrl = "";
	recepie.hasRunBtn = true;
	recepie.hasSingleBtn = true;
	recepie.hasPairSettingsBtn = true;
	recepie.hasPrintBtn = false;
	recepie.hasChannels = true;
	recepie.channelsPosition = scopy::gui::ChannelsPositionEnum::VERTICAL;

	m_monitorChannelManager = new scopy::gui::ChannelManager(recepie.channelsPosition);
	m_monitorChannelManager->setChannelIdVisible(false);

	m_toolView = scopy::gui::ToolViewBuilder(recepie,m_monitorChannelManager).build();

	m_generalSettingsMenu = generateMenu("General settings", new QColor("blue"));
	m_toolView->setGeneralSettingsMenu(m_generalSettingsMenu,false);

	m_dmmList=getDmmList(m_m2k_context);

	connect(m_toolView->getRunBtn(), SIGNAL(toggled(bool)), this, SLOT(toggleTimer(bool)));
	connect(m_toolView->getRunBtn(), SIGNAL(toggled(bool)),runButton(), SLOT(setChecked(bool)));
	connect(runButton(), SIGNAL(toggled(bool)), m_toolView->getRunBtn(),SLOT(setChecked(bool)));
	//	connect(m_toolView->getRunBtn(), SIGNAL(toggled(bool)),this, SLOT(startDataLogging(bool)));

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
	connect(m_toolView->getSingleBtn(), &QPushButton::toggled,this, [=](){
		testScaleFct();
	});

	connect(m_toolView->getSingleBtn(), &QPushButton::clicked,this, [=](bool checked){
		m_toolView->getSingleBtn()->setChecked(!checked);
		m_toolView->getRunBtn()->toggled(false);
	});




//	scopy::gui::ToolView *monitorsToolView =
			initMonitorToolView();

	//scopy::gui::ToolView *plotToolView = generatePlotToolView();

//	m_toolView->addFixedTabbedWidget(monitorsToolView,"Monitors",0,0,0,0,0);
//	m_toolView->addFixedTabbedWidget(plotToolView,"Plot",0,0,0,0,0);
	//m_toolView->addFixedTabbedWidget(generatePlotToolView(),"Plot 2",0,1,0,0,0);

	m_toolView->addFixedCentralWidget(m_fixedColGrid,0,0,0,0);
	m_toolView->setGeneralSettingsMenu(m_generalSettingsMenu,false);

	connect(m_timer, &QTimer::timeout , this, &IIOMonitor::readChannelValues);
	connect(this, &IIOMonitor::RecordingIntervalChanged, this , [=](double interval){
		VALUE_READING_TIME_INTERVAL = interval;
		m_timer->setInterval(interval);
	});
}

scopy::gui::ToolView* IIOMonitor::generatePlotToolView(){

	scopy::gui::ChannelManager* channelManager;
	scopy::gui::ToolView* toolView;
	PlotContainer* plotContainer = new PlotContainer(m_m2k_context,this);;

	scopy::gui::ToolViewRecipe plotRecepie;
	plotRecepie.hasHeader = false;
	plotRecepie.hasChannels = true;
	plotRecepie.channelsPosition = scopy::gui::ChannelsPositionEnum::VERTICAL;

	channelManager = new scopy::gui::ChannelManager(plotRecepie.channelsPosition);
	channelManager->setChannelIdVisible(false);
	toolView = scopy::gui::ToolViewBuilder(plotRecepie,channelManager).build();

	toolView->addFixedCentralWidget(plotContainer,0,0,0,0);

	int chId = 0;

	for(libm2k::analog::DMM* dmm : m_dmmList){
		std::vector<ChannelWidget*> channelList;
		scopy::gui::IIOMonitorMenu* menu = new scopy::gui::IIOMonitorMenu(this);
		menu->showAllMenu(true);
		menu->showChangeColor(false);
		menu->setTitle(QString::fromStdString(dmm->getName()),QColor("green"));

		ChannelWidget *ch_widget =
			toolView->buildNewChannel(channelManager, menu, true, chId, false, false, QColor("green"), QString::fromStdString(dmm->getName()), QString::fromStdString(dmm->getName()) );
		ch_widget->enableButton()->setChecked(false);

		 //////////////////
		 std::vector<VerticalControl*> m_verticalCtrls;
		 ///

		for(auto channel : dmm->readAll()){
		 QColor channelColor = getChannelColor(chId);
		  //scopy::gui::IIOMonitorMenu* m = new scopy::gui::IIOMonitorMenu(this);
		  //m->setTitle(QString::fromStdString(dmm->getName() + ": " + channel.id));

		 scopy::gui::GenericMenu* m = new scopy::gui::GenericMenu(this);

		  ///////////////// plot units/div and position controls
		  m->initInteractiveMenu();
		  m->setMenuHeader(QString::fromStdString(dmm->getName() + ": " + channel.id),new QColor(channelColor),false);
		  m_verticalCtrls.push_back(new VerticalControl({static_cast<int>(chId)}));
		  auto *verticalSection = new scopy::gui::SubsectionSeparator("", false,m);

		  auto *unitsPerDiv = new ScaleSpinButton({
													{"μUnits", 1E-6},
													{"mUnits", 1E-3},
													{"Units", 1E0},
													{"kUnits", 1E3}
											 }, tr("Units/Div"), 1e-4, 1e4,
											 true, false, this);
		  auto *verticalPosition  = new PositionSpinButton({
														  {"μUnits", 1E-6},
														  {"mUnits", 1E-3},
														  {"Units", 1E0}
												   }, tr("Position"),
												   -5*1e4, 5*1e4, true, false, this);
		  unitsPerDiv->setValue(1);
		  verticalPosition->setValue(0);

		  auto *autoWidget = new QWidget(this);
		  auto *layout = new QVBoxLayout(autoWidget);
		  layout->addWidget(new QLabel("Auto scale", this));
		  auto *autoSwitch = new CustomSwitch(this);
		  layout->addWidget(autoSwitch);

		  auto *setCurveActiveButton = new QPushButton(this);
		  setCurveActiveButton->setText("Set active");
		  setCurveActiveButton->setStyleSheet("QPushButton{"
											  "height:25px;"
											  "background-color: #4A64FF;"
											  "border-radius: 4px;"
											  "font-size: 12px;"
											  "line-height: 14px;"
											  "color: #FFFFFF;}"
											  "QPushButton:hover{"
											  "background-color: #4a34ff;"
											  "}");

		  layout->addWidget(setCurveActiveButton);

		  //verticalSection->setContent(menuTitle);
		  verticalSection->setContent(unitsPerDiv);
		  verticalSection->setContent(verticalPosition);
		  verticalSection->setContent(autoWidget);
		  verticalSection->getButton()->click();

		  m->insertSection(verticalSection);
		  m_verticalCtrls.back()->registerUnitsPerDivWidget(unitsPerDiv);
		  m_verticalCtrls.back()->registerPositionWidget(verticalPosition);
		  m_verticalCtrls.back()->registerAutoscaleBtn(autoSwitch);
		  m_verticalCtrls.back()->registerPlot(plotContainer->getPlot());

		  //////////////////

		  connect(setCurveActiveButton, &QPushButton::clicked, this, [=](){
			  plotContainer->setAxisActive(chId);
		  });

		  //TODO check why buttons are not added
		  /*
		  auto *buttonsSeparator = new scopy::gui::SubsectionSeparator("", false,m);
		  auto *setCurveActiveButton = new QPushButton(this);
		  setCurveActiveButton->setText("Set active");

		  buttonsSeparator->setContent(setCurveActiveButton);

		  auto *bringCurteFront = new QPushButton(this);
		  bringCurteFront->setText("Bring curve front");

		  buttonsSeparator->setContent(bringCurteFront);


		  connect(setCurveActiveButton, &QPushButton::clicked, this, [=](){
			  plotContainer->setAxisActive(chId);
		  });

		  connect(bringCurteFront, &QPushButton::clicked, this, [=](){
			  plotContainer->bringCurveToFront(chId);
		  });

		  m->insertSection(buttonsSeparator);

		  */

		  //endof TODO

		  ChannelWidget *ch_widget =
				 toolView->buildNewChannel(channelManager, m, true, chId, false, false, channelColor, QString::fromStdString(channel.id), QString::fromStdString(channel.id) );

		channelList.push_back(ch_widget);

		ch_widget->enableButton()->setChecked(false);
		ch_widget->hide();


		//connect widget to group settings (main menu)
		connect(menu, &scopy::gui::IIOMonitorMenu::toggleAll, this, [=](bool toggled){
			if(ch_widget->enableButton()->isChecked() != toggled){
			 ch_widget->enableButton()->click();
			}
		});

		// logic for enable/disable channels (curves)
		connect(ch_widget, &ChannelWidget::enabled,this, [=](bool enabled){
			if(enabled){
				activeChannels[chId].dmmId = channel.id;
				activeChannels[chId].dmm = dmm;
				activeChannels[chId].numberOfTabsUsing++; //increment number of tabs using this channel

				activeChannels[chId].chColor = channelColor;

				m_activePlots[plotContainer].push_back(chId);

			}else{
				//decrese number of tabs using this channel if no channel left remove from active list
				activeChannels[chId].numberOfTabsUsing--;
				if(activeChannels[chId].numberOfTabsUsing == 0){
					activeChannels.erase(activeChannels.find(chId));
				}

				for(int i = 0 ; i < m_activePlots[plotContainer].size() ; i++){
					if(m_activePlots[plotContainer].at(i) == chId){
						m_activePlots[plotContainer].remove(i);
					}
				}
			}
		});

		//TODO ?plot recording interval update
//		connect(m_generalSettingsMenu, &scopy::gui::IIOMonitorGeneralSettingsMenu::recordingTimeChanged,this, [=](double value){
//			monitor->setRecordingInterval(value/1000);
//		});

		   chId++;
		  }
		chId++;
		toolView->buildChannelGroup(channelManager, ch_widget,channelList);
	  }

	return toolView;
}

void IIOMonitor::initMonitorToolView(){

	int chId = 1;
	m_fixedColGrid = new MonitorContainer(this);
	m_fixedColGrid->setMaxColumnNumber(2);

	for(libm2k::analog::DMM* dmm : m_dmmList){
		std::vector<ChannelWidget*> channelList;

		scopy::gui::IIOMonitorGenericMenu *menu = new scopy::gui::IIOMonitorGenericMenu(this);
		menu->init(QString::fromStdString(dmm->getName()),new QColor("green"),true);

		ChannelWidget *mainCh_widget =
				m_toolView->buildNewChannel(m_monitorChannelManager, menu, true, chId, false, false, QColor("green"), QString::fromStdString(dmm->getName()), QString::fromStdString(dmm->getName()) );

		mainCh_widget->enableButton()->setChecked(false);
		chId++;
		for(auto channel : dmm->readAll()){

			QColor channelColor = getChannelColor(chId);

			scopy::gui::IIOMonitorGenericMenu *channelMenu = new scopy::gui::IIOMonitorGenericMenu(this);
			channelMenu->init(QString::fromStdString(dmm->getName() + ": " + channel.id),new QColor(channelColor),false);

			ChannelWidget *ch_widget =
					m_toolView->buildNewChannel(m_monitorChannelManager, channelMenu , true, chId, false, false, channelColor, QString::fromStdString(channel.id), QString::fromStdString(channel.id) );

			channelList.push_back(ch_widget);

			ch_widget->enableButton()->setChecked(false);
			ch_widget->hide();

			adiscope::ChannelMonitorComponent* monitor = new adiscope::ChannelMonitorComponent();
			monitor->setID(chId);
			monitor->init(0,QString::fromStdString(channel.unit_name),QString::fromStdString(channel.unit_symbol),QString::fromStdString(dmm->getName() + ": " + channel.id),channelColor );
			monitor->setChannelId(channel.id);
			monitor->setRecordingInterval(VALUE_READING_TIME_INTERVAL/1000);
			monitor->setHistoryDuration(10);

			//connect widget to group settings (main menu)
//			createConnections(menu,monitor);
//			connect(menu, &scopy::gui::IIOMonitorMenu::toggleAll, this, [=](bool toggled){
//				if(ch_widget->enableButton()->isChecked() != toggled){
//					ch_widget->enableButton()->click();
//				}
//			});

			//connect menu switches to widget
			createConnections(channelMenu,monitor);

			//add connections from general settings
			connect(this, &IIOMonitor::PrecisionChanged, this, [=](int precision){
				monitor->updateLcdNumberPrecision(precision);
			});

			connect(this, &IIOMonitor::RecordingIntervalChanged,this, [=](double value){
				monitor->setRecordingInterval(value/1000);
			});

			int widgetId =m_fixedColGrid->addQWidgetToList(monitor);

			// logic for enable/disable channels (monitors)
			connect(ch_widget, &ChannelWidget::enabled,this, [=](bool enabled){
				if(enabled){
					m_fixedColGrid->addWidget(widgetId);
					activeChannels[chId].dmmId = channel.id;
					activeChannels[chId].dmm = dmm;
					activeChannels[chId].numberOfTabsUsing++; //increment number of tabs using this channel

					m_activeMonitors[chId] = monitor;

				}else{

					//decrese number of tabs using this channel if no channel left remove from active list
					activeChannels[chId].numberOfTabsUsing--;
					if(activeChannels[chId].numberOfTabsUsing == 0){
						activeChannels.erase(activeChannels.find(chId));
					}
					m_activeMonitors.erase(m_activeMonitors.find(chId));
					m_fixedColGrid->removeWidget(widgetId);
				}
			});
			chId++;
		}

		m_toolView->buildChannelGroup(m_monitorChannelManager, mainCh_widget,channelList);
	}
}

scopy::gui::ToolView* IIOMonitor::getToolView(){
    return m_toolView;
}

std::vector<libm2k::analog::DMM*> IIOMonitor::getDmmList(libm2k::context::M2k* m2k_context){
    return m2k_context->getAllDmm() ;
}

void IIOMonitor::readChannelValues(){

	QMap<int,double> channelUpdatedValue;

	if(!activeChannels.empty()){
		for(auto ch : activeChannels.keys()){
			auto updatedRead = activeChannels[ch].dmm->readChannel(activeChannels[ch].dmmId);

			//handle monitors value update
			if(m_activeMonitors.contains(ch)){
				m_activeMonitors[ch]->updateValue(updatedRead.value,QString::fromStdString(updatedRead.unit_name), QString::fromStdString(updatedRead.unit_symbol));
			}

			channelUpdatedValue[ch] = updatedRead.value;
		}

		//EMIT plot updated values for each plot tab

		for(auto plot : m_activePlots.keys()){
			QMap<int,QPair<double,QColor>> updateVal;
			for(auto ch : m_activePlots[plot]){
				updateVal[ch] = qMakePair(channelUpdatedValue[ch],activeChannels[ch].chColor);
			}
			plot->updatePlot(updateVal);
		}
	}
}

void IIOMonitor::toggleTimer(bool enabled){

    // set timer for 5s
    if(enabled){
        readChannelValues();
        m_timer->start(VALUE_READING_TIME_INTERVAL);
    }else{
        m_timer->stop();
    }
}

void IIOMonitor::createConnections(scopy::gui::IIOMonitorGenericMenu* menu,adiscope::ChannelMonitorComponent* monitor){
	connect(menu,&scopy::gui::IIOMonitorGenericMenu::togglePeakHolder,this,[=](bool toggled){
	   monitor->displayPeakHold(toggled);
	});

	connect(menu, &scopy::gui::IIOMonitorGenericMenu::toggleHistory,this,[=](bool toggled){
		monitor->displayHistory(toggled);
	});

	connect(menu, &scopy::gui::IIOMonitorGenericMenu::toggleScale,this, [=](bool toggled){
	   monitor->displayScale(toggled);
	});

	connect(menu, &scopy::gui::IIOMonitorGenericMenu::resetPeakHolder, this, [=](){
	   monitor->resetPeakHolder();
	});

	connect(menu, &scopy::gui::IIOMonitorGenericMenu::monitorColorChanged,this, [=](QString color){
		monitor->setMonitorColor(color);
	});

	connect(menu, &scopy::gui::IIOMonitorGenericMenu::changeHistorySize, this, [=](double duration){
		monitor->setHistoryDuration(duration);
	});

	connect(menu, &scopy::gui::IIOMonitorGenericMenu::lineStyleChanged, this, [=](Qt::PenStyle style){
		monitor->setLineStyle(style);
	});

}

void IIOMonitor::testScaleFct(){
	//handle monitors value update
	if(m_activeMonitors.contains(0)){
		m_activeMonitors[0]->updateValue(testScale.at(i),QString::fromStdString("Volt"), QString::fromStdString("V"));
	}
	//QMap<QString, double> val;
	QMap<int,QPair<double,QColor>> val;
	val[100] = qMakePair(testScale.at(i), QColor("red"));
//	plotContainer->updatePlot(val);

    if ( i < testScale.size() - 1 ){
        i++;
    }else{
        i = 0;
    }
}

QColor IIOMonitor::generateColor(){

	int red = rand() % 256;
	int blue = rand() % 256;
	int green = rand() % 256;
	return QColor(red,green,blue);
}

QColor IIOMonitor::getChannelColor(int chId){
	if(m_color.contains(chId)){
		return m_color[chId];
	}else{
		if(m_color.isEmpty()){
			m_color[chId]= generateColor();
			return m_color[chId];
		}
		QColor color; //= generateColor();
		bool colorExists = true;
		while(colorExists){//m_color.contains(color)){
			color = generateColor();
			for(auto it = m_color.begin(); it != m_color.end(); it++){
				if(it.value() != color){
					colorExists = false;
					break;
				}
			}
		}
		m_color[chId]=color;
		//m_colors.push_back(color);
		return color;
	}
}


scopy::gui::GenericMenu* IIOMonitor::generateMenu(QString title, QColor* color){
	scopy::gui::GenericMenu *menu = new scopy::gui::GenericMenu(this);
	menu->initInteractiveMenu();
	menu->setMenuHeader(title,color,false);

	auto *precisionSection = new scopy::gui::SubsectionSeparator("Precision", false,this);

	QWidget *precisionWidget = new QWidget(this);
	auto *precisionLayout = new QHBoxLayout(precisionWidget);
	auto *precisionBtn = new QPushButton("Set Precision",precisionWidget);
	precisionBtn->setStyleSheet("QPushButton{"
										"height:25px;"
										"background-color: #4A64FF;"
										"border-radius: 4px;"
										"font-size: 12px;"
										"line-height: 14px;"
										"color: #FFFFFF;}"
										"QPushButton:hover{"
										"background-color: #4a34ff;"
										"}");

	auto *precisionValue = new QLineEdit(precisionWidget);
	precisionValue->setText("3");
	precisionLayout->addWidget(precisionBtn);
	precisionLayout->addWidget(precisionValue);

	connect(precisionBtn, &QPushButton::clicked,this, [=](){
		Q_EMIT PrecisionChanged(precisionValue->text().toInt());
	});


	precisionSection->setContent(precisionWidget);


	auto *recordingIntervalSection = new scopy::gui::SubsectionSeparator("Recording interval", false, this);

	auto recordingIntervalWidget = new QWidget(this);
	auto *recordingIntevlaLayout = new QVBoxLayout(recordingIntervalWidget);

	PositionSpinButton *recording_timer = new PositionSpinButton({
		{"s", 1},
		{"min", 60},
		{"h", 3600}
	}, tr("Timer"), 0, 3600,
	true, false, this);

	recording_timer->setValue(1);

	recordingIntevlaLayout->addWidget(recording_timer);

	connect(recording_timer, &PositionSpinButton::valueChanged, this, [=](){
		Q_EMIT RecordingIntervalChanged(recording_timer->value() * 1000);
	});

	recordingIntervalSection->setContent(recordingIntervalWidget);

	precisionSection->getButton()->click();
	recordingIntervalSection->getButton()->click();

	menu->insertSection(precisionSection);
	menu->insertSection(recordingIntervalSection);


	return menu;
}

IIOMonitor::~IIOMonitor()
{
    if(m_m2k_context){ delete m_m2k_context; }
	if(m_toolView){ delete m_toolView; }
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
