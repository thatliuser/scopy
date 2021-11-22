#include "dynamicWidget.hpp"

#include <QHBoxLayout>

#include "channel_manager.hpp"

using namespace scopy::gui;
using namespace adiscope;

ChannelManager::ChannelManager(ChannelsPositionEnum position, QWidget* parent)
	: QWidget(parent)
	, m_scrollArea(new QScrollArea(parent))
	, m_channelsWidget(new QWidget(m_scrollArea))
	, m_hasAddBtn(false)
	, m_addChannelBtn(new CustomPushButton()) //check if parent removed
    , m_position(position)
    , m_channelIdVisible(true)

{
	if (m_position == ChannelsPositionEnum::VERTICAL) {
		m_channelsWidget->setLayout(new QVBoxLayout(m_channelsWidget));

		m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


		channelMnagerToggled = false;
		toggleChannels = new QPushButton(this);
		toggleChannels->setFixedWidth(20);
		toggleChannels->setCheckable(true);
		toggleChannels->setChecked(true);

		connect(toggleChannels, &QPushButton::clicked, this, &ChannelManager::toggleChannelManager);
		m_channelsWidget->layout()->addWidget(toggleChannels);

	} else {
		m_channelsWidget->setLayout(new QHBoxLayout(m_channelsWidget));

		m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}

	m_channelsWidget->layout()->setSpacing(0);
	m_channelsWidget->layout()->setMargin(0);
	m_channelsWidget->layout()->setContentsMargins(QMargins(0,0,0,0));
	m_channelsWidget->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);

	m_scrollArea->setWidget(m_channelsWidget);

    //m_scrollArea->setStyleSheet("background-color:orange");
    //m_channelsWidget->setStyleSheet("background-color:red");
      //  m_scrollArea->setSizePolicy(m_channelsWidget->sizePolicy());



}

ChannelManager::~ChannelManager()
{
	delete m_channelsWidget;
	if (m_addChannelBtn) {
		delete m_addChannelBtn;
	}
	delete m_parent;
}

void ChannelManager::build(QWidget* parent)
{
	m_parent = parent;
	m_parent->layout()->addWidget(m_scrollArea);
}

ChannelWidget* ChannelManager::buildNewChannel(int chId, bool deletable, bool simplefied, QColor color,
                           const QString& fullName, const QString& shortName)
{
	ChannelWidget* ch = new ChannelWidget(chId, deletable, simplefied, color);
	m_channelsWidget->layout()->setMargin(0);

    m_channelsWidget->layout()->addWidget(ch);
    if(m_channelIdVisible){
        ch->setFullName(fullName + QString(" %1").arg(chId + 1));
        ch->setShortName(shortName + QString(" %1").arg(chId + 1));
    }else{
        ch->setFullName(fullName);
        ch->setShortName(shortName);
    }
	ch->nameButton()->setText(ch->shortName());

	m_channelsList.append(ch);

	if (m_position == ChannelsPositionEnum::VERTICAL) {
		m_channelsWidget->setMinimumHeight(m_channelsList.size() * m_channelsList.first()->height());
		m_channelsWidget->setMaximumHeight(m_channelsList.size() * m_channelsList.first()->height());

		m_channelsWidget->setMinimumWidth(m_channelsList.last()->width());
		m_channelsWidget->setMaximumWidth(m_channelsList.last()->width());
	} else {
		m_channelsWidget->setMinimumWidth(m_channelsList.size() * m_channelsList.first()->width());
		m_channelsWidget->setMaximumWidth(m_channelsList.size() * m_channelsList.first()->width());

		m_channelsWidget->setMinimumHeight(m_channelsList.first()->height());
		m_channelsWidget->setMaximumHeight(m_channelsList.first()->height());
	}

	return ch;
}

void ChannelManager::removeChannel(ChannelWidget* ch)
{
	m_channelsList.removeOne(ch);
	m_channelsWidget->layout()->removeWidget(ch);
	delete ch;
}

CustomPushButton* ChannelManager::getAddChannelBtn()
{
	if (m_hasAddBtn) {
		return m_addChannelBtn;
	} else {
		return nullptr;
	}
}

QList<ChannelWidget*> ChannelManager::getChannelsList() { return m_channelsList; }

void ChannelManager::changeParent(QWidget* newParent)
{
	delete m_channelsWidget->layout();

	if (m_position == ChannelsPositionEnum::VERTICAL) {
		m_channelsWidget->setLayout(new QVBoxLayout(m_channelsWidget));

		m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	} else {
		m_channelsWidget->setLayout(new QHBoxLayout(m_channelsWidget));

		m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}

	m_channelsWidget->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);
	m_channelsWidget->layout()->setSpacing(0);
	m_channelsWidget->layout()->setMargin(0);

	for (ChannelWidget* channel : m_channelsList) {
		m_channelsWidget->layout()->addWidget(channel);
	}

	if (m_position == ChannelsPositionEnum::VERTICAL) {
		m_channelsWidget->setMinimumHeight(m_channelsList.size() * m_channelsList.first()->height());
		m_channelsWidget->setMaximumHeight(m_channelsList.size() * m_channelsList.first()->height());

		m_channelsWidget->setMinimumWidth(m_channelsList.last()->width() + 25);
		m_channelsWidget->setMaximumWidth(m_channelsList.last()->width() + 25);
	} else {
		m_channelsWidget->setMinimumWidth(m_channelsList.size() * m_channelsList.first()->width());
		m_channelsWidget->setMaximumWidth(m_channelsList.size() * m_channelsList.first()->width());

		m_channelsWidget->setMinimumHeight(m_channelsList.first()->height());
		m_channelsWidget->setMaximumHeight(m_channelsList.first()->height());
	}

	m_parent = newParent;
	m_parent->layout()->addWidget(m_scrollArea);
	m_parent->layout()->addWidget(m_addChannelBtn);
}

QList<ChannelWidget*> ChannelManager::getEnabledChannels()
{
	QList<ChannelWidget*> lst;

	for (auto widget : m_channelsWidget->children()) {
		ChannelWidget* channel = qobject_cast<ChannelWidget*>(widget);
		if (channel->enableButton()->isChecked()) {
			lst.append(channel);
		}
	}

	return lst;
}

void ChannelManager::insertAddBtn(QWidget* menu, bool dockable)
{

	m_hasAddBtn = true;
	m_addChannelBtn = new CustomPushButton(m_scrollArea);

    // TO DO: center + btn when position is vertical
	m_addChannelBtn->setCheckable(true);
	m_addChannelBtn->setFlat(true);

    m_addChannelBtn->setIcon(QIcon(":/menu/add.svg"));
    m_addChannelBtn->setIconSize(QSize(25, 25));
    m_addChannelBtn->setMaximumSize(25, 25);


    m_parent->layout()->addWidget(m_addChannelBtn);
    m_parent->layout()->setAlignment(m_addChannelBtn,Qt::AlignHCenter);

	Q_EMIT(configureAddBtn(menu, dockable));
}

void ChannelManager::setChannelAlignment(ChannelWidget* ch, Qt::Alignment alignment){   
    m_channelsWidget->layout()->setAlignment(ch,alignment);
}

void ChannelManager::setChannelIdVisible(bool visible){
    m_channelIdVisible = visible;
}

void ChannelManager::toggleChannelManager(bool toggled){
	channelMnagerToggled = !channelMnagerToggled;
	Q_EMIT channelManagerToggled(toggled);

	for(auto ch : m_channelsList){
		ch->toggleChannel(channelMnagerToggled);
		if(ch->isMainChannel()){
			ch->setMenuButtonVisibility(toggled);
		}
	}
}
