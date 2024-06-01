#include "tabGroup.h"
#include "tab.h"

#include <algorithm>

TabGroup::TabGroup(QWebEngineProfile *profile, QWidget *parent): QWidget(parent){
    this->profile = profile;
    Tab *tab = new Tab(profile);
    Tab *another = new Tab(profile);
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

    emit this->reachedSingleTab(this->tabs.at(0));

    QObject::connect(this, &TabGroup::tabsChanged, this, &TabGroup::handleTitleBars);
}

void TabGroup::insertTab(Tab *tab, int pos){
    this->tabs.insert(this->tabs.begin()+pos, tab);
    this->tabSplitter->insertWidget(pos, tab);
}

Tab* TabGroup::getTab(int pos){
    return this->tabs.at(pos);
}

void TabGroup::splitLeft(Tab *tab){
    if(this->tabs.size() == 4){
        return;
    }

    std::vector<Tab*>::iterator tabIter = std::find(this->tabs.begin(), this->tabs.end(), tab);
    const int pos = std::distance(this->tabs.begin(), tabIter);
    this->insertTab(new Tab(this->profile), pos);

    emit this->tabsChanged(this->tabs.size());
}

void TabGroup::splitRight(Tab *tab){
    if(this->tabs.size() == 4){
        return;
    }

    std::vector<Tab*>::iterator tabIter = std::find(this->tabs.begin(), this->tabs.end(), tab);
    const int pos = std::distance(this->tabs.begin(), tabIter);
    this->insertTab(new Tab(this->profile), pos+1);

    emit this->tabsChanged(this->tabs.size());
}

void TabGroup::handleTitleBars(int count){
    for(Tab* tab: this->tabs){
        tab->setTitleBarVisible(count > 1);
    }

    if(this->tabs.size() == 1){
        emit this->reachedSingleTab(this->tabs.at(0));
    }
}

TabGroup::~TabGroup(){
    for(Tab* tab: this->tabs){
        delete tab;
    }

    delete this->tabSplitter;
    delete this->splitScreenLayout;
}