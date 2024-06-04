#include "splitTabMenu.h"
#include "iconButton.h"

#include <QWidgetAction>
#include <QHBoxLayout>

SplitTabMenu::SplitTabMenu(QObject *parent): QWidgetAction(parent){
    this->actionWidget = new QWidget();
    this->layout = new QHBoxLayout(this->actionWidget);

    this->splitLeftButton = new IconButton(":/icons/split-left.png");
    this->splitRightButton = new IconButton(":/icons/split-right.png");

    this->layout->addWidget(this->splitLeftButton);
    this->layout->addWidget(this->splitRightButton);
    this->setDefaultWidget(this->actionWidget);
}

SplitTabMenu::~SplitTabMenu(){
    
}