#include "tabGroup.h"
#include "tab.h"

#include <algorithm>

TabGroup::TabGroup(QWidget *parent): QWidget(parent){
    Tab *tab = new Tab();
    this->tabs.push_back(tab);

    this->tabSplitter = new QSplitter();
    this->tabSplitter->setHandleWidth(5);
    this->tabSplitter->setStyleSheet(
        "QSplitter::handle{"
        "   background: transparent;"
        "}"
    );

    this->splitScreenLayout = new QHBoxLayout(this);

    this->tabSplitter->addWidget(tab);
    this->splitScreenLayout->addWidget(this->tabSplitter);
    
    // this->splitScreenLayout->setSpacing(5);
    this->splitScreenLayout->setContentsMargins(0,0,0,0);
    // splitScreenLayout->addWidget(tab);

    QObject::connect(this, &TabGroup::tabsChanged, this, &TabGroup::handleTitleBars);
}

void TabGroup::insertTab(Tab *tab, int pos){
    this->tabs.insert(this->tabs.begin()+pos, tab);
    this->tabSplitter->insertWidget(pos, tab);
}

void TabGroup::splitLeft(Tab *tab){
    if(this->tabs.size() == 4){
        return;
    }

    std::vector<Tab*>::iterator tabIter = std::find(this->tabs.begin(), this->tabs.end(), tab);
    const int pos = std::distance(this->tabs.begin(), tabIter);
    this->insertTab(new Tab(), pos);

    emit this->tabsChanged(this->tabs.size());
}

void TabGroup::splitRight(Tab *tab){
    if(this->tabs.size() == 4){
        return;
    }

    std::vector<Tab*>::iterator tabIter = std::find(this->tabs.begin(), this->tabs.end(), tab);
    const int pos = std::distance(this->tabs.begin(), tabIter);
    this->insertTab(new Tab(), pos+1);

    emit this->tabsChanged(this->tabs.size());
}

void TabGroup::handleTitleBars(int count){
    for(Tab* tab: this->tabs){
        tab->setTitleBarVisible(count > 1);
    }
}

TabGroup::~TabGroup(){
    for(Tab* tab: this->tabs){
        delete tab;
    }

    delete this->tabSplitter;
    delete this->splitScreenLayout;
}