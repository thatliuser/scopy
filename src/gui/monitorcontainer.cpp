#include "monitorcontainer.hpp"
#include "ui_monitorcontainer.h"
#include "QScrollArea"
#include <QSpacerItem>
#include <QtDebug>

MonitorContainer::MonitorContainer(int maxCols,QWidget *parent) :
	QWidget(parent),
	m_maxCols(maxCols-1),
	currentNumberOfCols(m_maxCols),
	col(0),
	row(0),
	ui(new Ui::MonitorContainer)
{
	ui->setupUi(this);

	m_mainWidget = new QWidget(this);
	ui->scrollArea->setWidgetResizable(true);
	ui->scrollArea->setWidget(m_mainWidget);
	m_gridLayout = new QGridLayout;
	m_gridLayout->setHorizontalSpacing(0);
	m_gridLayout->setVerticalSpacing(0);
	m_gridLayout->setContentsMargins(0, 0, 0, 0);
	m_gridLayout->setSpacing(0);
	m_gridLayout->setMargin(0);
	m_mainWidget->setLayout(m_gridLayout);

	m_hspacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_vspacer = new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);

	m_gridLayout->addItem(m_hspacer,0, 1);
	m_gridLayout->addItem(m_vspacer,1, 0);



}

// adds widget to internal widget list and return the index of the added widget
int MonitorContainer::addQWidgetToList(QWidget *widget){
	m_widgetList.push_back(widget);
	return  m_widgetList.size() -1;
}

// adds a widget at index to the layout and it's index to the active widget list
void MonitorContainer::addWidget(int index){
	m_gridLayout->addWidget(m_widgetList.at(index),row,col);
	m_widgetList.at(index)->show();
	m_activeWidgetList.push_back(index);

	if(m_activeWidgetList.size() == 2)
	{
		m_gridLayout->removeItem(m_hspacer);
	}
	if(m_activeWidgetList.size() == 3)
	{
		m_gridLayout->removeItem(m_vspacer);
	}
	//logic for resizable n columns grid layout
	if(col == currentNumberOfCols){
		col = 0;
		row++;
	}else{
		col ++;
	}

}


// remove widget at index from layout
void MonitorContainer::removeWidget(int index){
	//get index, row and column of item that will be removed
	int indexOfGrid = m_gridLayout->indexOf(m_widgetList.at(index));
	int r = 0, c=0, rs = 0, cs =0;
	m_gridLayout->getItemPosition(indexOfGrid,&r,&c,&rs,&cs);

	//remove the widget from the layout and hide it (use hide instead of delete since widget can be added again)
	m_gridLayout->removeWidget(m_widgetList.at(index));
	m_widgetList.at(index)->hide();

	int i = 0;
	for(int idx = 0; idx < m_activeWidgetList.size(); idx++){
		if(m_activeWidgetList.at(idx) == index){
			i = idx;
			m_activeWidgetList.remove(idx);
			break;
		}
	}

	//shift all remaing active widgets to fill the empty space
	while(i < m_activeWidgetList.size()){
		repositionWidgets(m_activeWidgetList.at(i),r,c);
		i++;
		if(c == currentNumberOfCols){
			c = 0;
			r++;
		}else{
			c ++;
		}
	}

	if(m_activeWidgetList.size() == 1){
		m_gridLayout->addItem(m_hspacer,0, 1);
	}
	if(m_activeWidgetList.size() == 2){
		m_gridLayout->addItem(m_vspacer,1, 0);
	}
	//logic for n columns grid layout
	if(col != 0){
		col--;
	}else{
		row--;
		col = currentNumberOfCols;
	}
}

// move a widget to specified row and column
// moving the widget will replace the current widget at that position
void MonitorContainer::repositionWidgets(int index, int row, int col){
	m_gridLayout->addWidget(m_widgetList.at(index),row,col);
}


// returns widget at index
QWidget* MonitorContainer::getWidget(int index){
	return m_widgetList.at(index);
}

// check if widget is active on layout
bool MonitorContainer::isWidgetActive(int index){
	for(int i = 0 ; i < m_activeWidgetList.size(); i++){
		if(m_activeWidgetList.at(i) == index){
			return true;
		}
	}
	return false;
}

void MonitorContainer::toggleAll(bool toggled){
	for(int i=0; i< m_widgetList.size(); i++){
		if(toggled){
			addWidget(i);
		}else{
			removeWidget(i);
		}
	}
}

void MonitorContainer::setMaxColumnNumber(int maxColumns){
	m_maxCols = maxColumns - 1;
}
int MonitorContainer::getMaxColumnNumber(){return  m_maxCols;}


void MonitorContainer::resizeEvent(QResizeEvent *event)
{
	auto widgetWidth = m_widgetList.at(0)->minimumWidth();
	auto newWidth = event->size().width();

	auto colCount = 0;

	if(widgetWidth != 0){
	 colCount = newWidth/widgetWidth - 1;
	}

	if(colCount > m_maxCols){
		colCount = m_maxCols;
	}

	if(colCount != currentNumberOfCols){

		currentNumberOfCols = colCount;
	}

	if(m_activeWidgetList.size() > 1){
		redrawWidgets();
	}

	QWidget::resizeEvent(event);
}

void MonitorContainer::redrawWidgets()
{
	row = 0;
	col = 0;
	if(m_activeWidgetList.size() > 0){
		for(int i = 0; i < m_activeWidgetList.size(); i++){
			m_gridLayout->removeWidget(m_widgetList.at(i));
			m_widgetList.at(i)->hide();
		}
		for(int i = 0; i < m_activeWidgetList.size(); i++){
			m_gridLayout->addWidget(m_widgetList.at(i),row,col);
			m_widgetList.at(i)->show();

			//logic for resizable n columns grid layout
			if(col == currentNumberOfCols){
				col = 0;
				row++;
			}else{
				col ++;
			}
		}

	}

}

MonitorContainer::~MonitorContainer()
{
	for(auto widget: m_widgetList){
		delete widget;
	}
	if(m_gridLayout){
		m_gridLayout->removeItem(m_vspacer);
		m_gridLayout->removeItem(m_hspacer);
		delete m_gridLayout;
	}
	delete ui;
}
