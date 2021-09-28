#include "monitorcontainer.hpp"
#include "ui_monitorcontainer.h"
#include "QScrollArea"

MonitorContainer::MonitorContainer(QWidget *parent) :
    QWidget(parent),
    m_maxCols(Q_INFINITY),
    col(0),
    row(0),
    ui(new Ui::MonitorContainer)
{
    ui->setupUi(this);

    QWidget *test = new QWidget;
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setWidget(test);
    m_gridLayout = new QGridLayout;
    test->setLayout(m_gridLayout);
}

// adds widget to internal widget list and return the index of the added widget
int MonitorContainer::addMonitorContainerToList(QWidget *widget){

    m_widgetList.push_back(widget);
    return  m_widgetList.size() -1;
}

// adds a widget at index to the layout and it's index to the active widget list
void MonitorContainer::addWidget(int index){

    //add widget from intex to next available position based on row and column
    auto gridLayout = dynamic_cast<QGridLayout*>(ui->scrollArea->widget()->layout());
    gridLayout->addWidget(m_widgetList.at(index),row,col);
    m_widgetList.at(index)->show();
    m_activeWidgetList.push_back(index);

    //logic for n columns grid layout
    if(col == m_maxCols){
        col = 0;
        row++;
    }else{
        col ++;
    }

}

// remove widget at index from layout
void MonitorContainer::removeWidget(int index){

     auto gridLayout = dynamic_cast<QGridLayout*>(ui->scrollArea->widget()->layout());

     //get index, row and column of item that will be removed
     int indexOfGrid = gridLayout->indexOf(m_widgetList.at(index));
     int r = 0, c=0, rs = 0, cs =0;
     gridLayout->getItemPosition(indexOfGrid,&r,&c,&rs,&cs);

     //shift all remaing active widgets to fill the empty space
     int i = indexOfGrid + 1;
     while(i < m_activeWidgetList.size()){
         repositionWidgets(m_activeWidgetList.at(i),r,c);
         i++;
         if(c == m_maxCols){
             c = 0;
             r++;
         }else{
             c ++;
         }
     }

     //remove the widget from the layout and hide it (use hide instead of delete since widget can be added again)
     gridLayout->removeWidget(m_widgetList.at(index));
     m_widgetList.at(index)->hide();

     m_activeWidgetList.erase(m_activeWidgetList.begin() + indexOfGrid );

   //logic for n columns grid layout
    if(col != 0){
        col--;
    }else{
        row--;
        col = m_maxCols;
    }


}

// move a widget to specified row and column
// moving the widget will replace the current widget at that position
void MonitorContainer::repositionWidgets(int index, int row, int col){

    auto gridLayout = dynamic_cast<QGridLayout*>(ui->scrollArea->widget()->layout());
    gridLayout->addWidget(m_widgetList.at(index),row,col);

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

MonitorContainer::~MonitorContainer()
{
    for(auto widget: m_widgetList){
        delete widget;
    }

    if(m_gridLayout){
        delete m_gridLayout;
    }

    delete ui;
}
