#include "tabManager.h"
#include "tabGroup.h"
#include <QHBoxLayout>
#include <QWidget>

TabManager::TabManager(QWidget *parent): QStackedWidget(parent){
    this->profile = new QWebEngineProfile();

    TabGroup *initialGroup = new TabGroup(this->profile);
    this->groups.push_back(initialGroup);

    //Temporary until multiple groups
    connect(initialGroup, &TabGroup::reachedSingleTab, this, [=](Tab* tab){
        emit this->embedTabTitlebar(tab->getTitleBar());
    });

    connect(initialGroup, &TabGroup::tabsChanged, this, [=](int count){
        if(count == 2){
            emit this->removeTabTitleBar(initialGroup->getMainTab());
        }
    });

    this->addWidget(initialGroup);
}

TabGroup* TabManager::getTabGroup(int pos){
    return this->groups.at(pos);
}

TabManager::~TabManager(){
    for(TabGroup* group: this->groups){
        delete group;
    }
    delete this->profile;
}