#include "dataloggergenericmenu.hpp"

#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "gui/customSwitch.hpp"

using namespace scopy::gui;

DataLoggerGenericMenu::DataLoggerGenericMenu(QWidget *parent):
	GenericMenu(parent)
{
	connect(this, &DataLoggerGenericMenu::historySizeIndexChanged, this, &DataLoggerGenericMenu::setHistorySize );
	connect(this, &DataLoggerGenericMenu::lineStyleIndexChanged, this, &DataLoggerGenericMenu::changeLineStyle );
}

DataLoggerGenericMenu::~DataLoggerGenericMenu()
{}

void DataLoggerGenericMenu::init(QString title, QColor* color)
{
	this->initInteractiveMenu();
	this->setMenuHeader(title,color,false);

	auto *scaleSection = new scopy::gui::SubsectionSeparator("Settings", false,this);

	//scale
	QWidget *scaleWidget = new QWidget(this);
	auto scaleLayout = new QHBoxLayout(scaleWidget);
	scaleSWitch = new CustomSwitch(scaleWidget);
	scaleSWitch->setChecked(true);

	scaleLayout->addWidget(new QLabel("Scale",scaleWidget));
	scaleLayout->addWidget(scaleSWitch);

	scaleSection->setContent(scaleWidget);

	////history
	auto *historyWidget = new QWidget(this);
	auto *historyLayout = new QVBoxLayout(historyWidget);
	auto *h1layout = new QHBoxLayout(historyWidget);

	historySwitch = new CustomSwitch(historyWidget);
	historySwitch->setChecked(true);
	historyLayout->addWidget(historySwitch);
	historyLayout->setAlignment(historySwitch,Qt::AlignRight);

	historySize = new QComboBox(historyWidget);
	historySize->addItem(QString("1s"));
	historySize->addItem(QString("10s"));
	historySize->addItem(QString("60s"));
	historySize->setCurrentIndex(1);

	auto *historySizeLabel = new QLabel("History Size",historyWidget);
	h1layout->addWidget(historySizeLabel);
	h1layout->addWidget(historySize);

	historyLayout->addItem(h1layout);

	auto *h2layout = new QHBoxLayout(historyWidget);
	historyStyle = new QComboBox(historyWidget);
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

	////Peak holder
	auto *peakHolderWidget = new QWidget(this);
	auto *peakHolderLayout = new QHBoxLayout(peakHolderWidget);
	peakHolderSwitch = new CustomSwitch(peakHolderWidget);
	peakHolderSwitch->setChecked(true);
	peakHolderReset = new QPushButton("Reset",peakHolderWidget);
	peakHolderReset->setProperty("blue_button",true);
	peakHolderReset->setMinimumHeight(30);

	peakHolderLayout->addWidget(peakHolderReset);
	peakHolderLayout->addWidget(peakHolderSwitch);

	auto *peakHolderSection = new scopy::gui::SubsectionSeparator("Peak Holder", true,this);

	peakHolderSection->setContent(peakHolderWidget);

	this->insertSection(scaleSection);
	this->insertSection(historySection);
	this->insertSection(peakHolderSection);

	this->layout()->addItem( new QSpacerItem(10,10, QSizePolicy::Fixed, QSizePolicy::Expanding));

	connect(scaleSWitch, &CustomSwitch::toggled, this, &DataLoggerGenericMenu::toggleScale);
	connect(historySwitch,  &CustomSwitch::toggled, this, &DataLoggerGenericMenu::toggleHistory);
	connect(peakHolderReset, &QPushButton::clicked,this, &DataLoggerGenericMenu::resetPeakHolder);
	connect(peakHolderSwitch, &CustomSwitch::toggled, this, &DataLoggerGenericMenu::togglePeakHolder);
	connect(historySize, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DataLoggerGenericMenu::historySizeIndexChanged);
	connect(historyStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DataLoggerGenericMenu::lineStyleIndexChanged);
}

void DataLoggerGenericMenu::scaleToggle(bool toggled)
{
	scaleSWitch->setChecked(toggled);
}

void DataLoggerGenericMenu::peakHolderToggle(bool toggled)
{
	peakHolderSwitch->setChecked(toggled);
}

void DataLoggerGenericMenu::historyToggle(bool toggled)
{
	historySwitch->setChecked(toggled);
}

void DataLoggerGenericMenu::peakHolderResetClicked()
{
	peakHolderReset->click();
}

void DataLoggerGenericMenu::historySizeChanged(int size)
{
	historySize->setCurrentIndex(size);
	setHistorySize(size);
}

void DataLoggerGenericMenu::changeLineStyle(int index)
{
	historyStyle->setCurrentIndex(index);
	Q_EMIT lineStyleChanged(lineStyleFromIdx(index));
}

void DataLoggerGenericMenu::setHistorySize(int idx)
{
	Q_EMIT changeHistorySize(numSamplesFromIdx(idx));
}

double DataLoggerGenericMenu::numSamplesFromIdx(int idx)
{
	switch(idx) {
	case 0:	return 1.0;
	case 1:	return 10.0;
	case 2:	return 60.0;
	default:throw std::runtime_error("Invalid INDEX");
	}
}

Qt::PenStyle DataLoggerGenericMenu::lineStyleFromIdx(int idx)
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

