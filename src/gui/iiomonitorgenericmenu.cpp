#include "iiomonitorgenericmenu.hpp"

#include <QLineEdit>

using namespace scopy::gui;

IIOMonitorGenericMenu::IIOMonitorGenericMenu(QWidget *parent):
	GenericMenu(parent)
{

}

IIOMonitorGenericMenu::~IIOMonitorGenericMenu()
{

}

void IIOMonitorGenericMenu::init(QString title, QColor* color, bool showAll){
	this->initInteractiveMenu();
	this->setMenuHeader(title,color,false);

	auto *scaleSection = new scopy::gui::SubsectionSeparator("Settings", false,this);

	//show all
	if(showAll){
		QWidget *showAllWidget = new QWidget(this);
		auto showAllLayout = new QHBoxLayout(showAllWidget);
		auto showAllSWitch = new CustomSwitch(showAllWidget);

		showAllLayout->addWidget(new QLabel("Show all",showAllWidget));
		showAllLayout->addWidget(showAllSWitch);

		scaleSection->setContent(showAllWidget);
	}

	//scale
	QWidget *scaleWidget = new QWidget(this);
	auto scaleLayout = new QHBoxLayout(scaleWidget);
	auto scaleSWitch = new CustomSwitch(scaleWidget);
	scaleSWitch->setChecked(true);

	scaleLayout->addWidget(new QLabel("Scale",scaleWidget));
	scaleLayout->addWidget(scaleSWitch);

	scaleSection->setContent(scaleWidget);

	//change color
	QWidget *changeColorWidget = new QWidget(this);
	auto *changeColorLayout = new QHBoxLayout(changeColorWidget);
	auto *changeColorBtn = new QPushButton("Set color",changeColorWidget);
	auto *colorValue = new QLineEdit(changeColorWidget);
	changeColorBtn->setStyleSheet("QPushButton{"
										"height:25px;"
										"background-color: #4A64FF;"
										"border-radius: 4px;"
										"font-size: 12px;"
										"line-height: 14px;"
										"color: #FFFFFF;}"
										"QPushButton:hover{"
										"background-color: #4a34ff;"
										"}");

	changeColorLayout->addWidget(changeColorBtn);
	changeColorLayout->addWidget(colorValue);

	scaleSection->setContent(changeColorWidget);

	////history
	auto *historyWidget = new QWidget(this);
	auto *historyLayout = new QVBoxLayout(historyWidget);
	auto *h1layout = new QHBoxLayout(historyWidget);

	auto *historySwitch = new CustomSwitch(historyWidget);
	historySwitch->setChecked(true);
	historyLayout->addWidget(historySwitch);
	historyLayout->setAlignment(historySwitch,Qt::AlignRight);

	QComboBox *historySize = new QComboBox(historyWidget);
	historySize->addItem(QString("1s"));
	historySize->addItem(QString("10s"));
	historySize->addItem(QString("60s"));
	historySize->setCurrentIndex(1);

	auto *historySizeLabel = new QLabel("History Size",historyWidget);
	h1layout->addWidget(historySizeLabel);
	h1layout->addWidget(historySize);

	historyLayout->addItem(h1layout);

	auto *h2layout = new QHBoxLayout(historyWidget);
	QComboBox *historyStyle = new QComboBox(historyWidget);
	historyStyle->addItem(QString("Solid Line"));
	historyStyle->addItem(QString("Dot Line"));
	historyStyle->addItem(QString("Dash Line"));
	historyStyle->addItem(QString("Dash-Dot Line"));
	historyStyle->addItem(QString("Dash-Dot-Dot Line"));
	historyStyle->setCurrentIndex(0);

	auto *historyStyleLabel = new QLabel("History Style",historyWidget);

	h2layout->addWidget(historyStyleLabel);
	h2layout->addWidget(historyStyle);
	historyLayout->addItem(h2layout);

	auto *historySection = new scopy::gui::SubsectionSeparator("History", true,this);
	historySection->setContent(historyWidget);
	////end history

	////Peak holder
	auto *peakHolderWidget = new QWidget(this);
	auto *peakHolderLayout = new QHBoxLayout(peakHolderWidget);
	auto *peakHolderSwitch = new CustomSwitch(peakHolderWidget);
	peakHolderSwitch->setChecked(true);
	auto *peakHolderReset = new QPushButton("Reset",peakHolderWidget);
	peakHolderReset->setStyleSheet("QPushButton{"
										"height:25px;"
										"background-color: #4A64FF;"
										"border-radius: 4px;"
										"font-size: 12px;"
										"line-height: 14px;"
										"color: #FFFFFF;}"
										"QPushButton:hover{"
										"background-color: #4a34ff;"
										"}");

	peakHolderLayout->addWidget(peakHolderReset);
	peakHolderLayout->addWidget(peakHolderSwitch);



	auto *peakHolderSection = new scopy::gui::SubsectionSeparator("Peak Holder", true,this);
	peakHolderSection->setContent(peakHolderWidget);
	///end peak holder


	scaleSection->getButton()->click();
	historySection->getButton()->click();
	peakHolderSection->getButton()->click();

	this->insertSection(scaleSection);
	this->insertSection(historySection);
	this->insertSection(peakHolderSection);

	this->layout()->addItem( new QSpacerItem(10,10, QSizePolicy::Fixed, QSizePolicy::Expanding));
	connect(scaleSWitch, &CustomSwitch::toggled, this, [=](bool toggled){
		Q_EMIT IIOMonitorGenericMenu::toggleScale(toggled);
	});
	connect(changeColorBtn, &QPushButton::clicked,this, [=](){
	   Q_EMIT monitorColorChanged(colorValue->text());
	});
	connect(historySwitch,  &CustomSwitch::toggled, this, [=](bool toggled){
		Q_EMIT IIOMonitorGenericMenu::toggleHistory(toggled);
	});
	connect(historySize, SIGNAL(currentIndexChanged(int)), this, SLOT(setHistorySize(int)));
	connect(historyStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
		Q_EMIT lineStyleChanged(lineStyleFromIdx(index));
	});
	connect(peakHolderReset, &QPushButton::clicked,this, [=](){
		Q_EMIT IIOMonitorGenericMenu::resetPeakHolder();
	});
	connect(peakHolderSwitch, &CustomSwitch::toggled, this, [=](bool toggled){
		Q_EMIT IIOMonitorGenericMenu::togglePeakHolder(toggled);
	});
}

void IIOMonitorGenericMenu::setHistorySize(int idx)
{
	Q_EMIT changeHistorySize(numSamplesFromIdx(idx));
}

double IIOMonitorGenericMenu::numSamplesFromIdx(int idx)
{
	switch(idx) {
	case 0:	return 1.0;
	case 1:	return 10.0;
	case 2:	return 60.0;
	default:throw std::runtime_error("Invalid INDEX");
	}
}

Qt::PenStyle IIOMonitorGenericMenu::lineStyleFromIdx(int idx)
{
	switch (idx) {
	case 0:	return Qt::PenStyle::SolidLine;
	case 1:	return Qt::PenStyle::DotLine;
	case 2:	return Qt::PenStyle::DashLine;
	case 3:	return Qt::PenStyle::DashDotLine;
	case 4: return Qt::PenStyle::DashDotDotLine;
	}
	return Qt::PenStyle::SolidLine;
}

