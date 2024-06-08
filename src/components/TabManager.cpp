#include "TabManager.h"

TabManager::TabManager(QWidget *parent): QStackedWidget(parent), currentGroup(0){
    this->profile = new QWebEngineProfile();
    this->groups.push_back(new TabGroup(profile));

    this->connect(this->groups.at(0), &TabGroup::tabsChanged, this, [=](){
        if(this->groups.at(0)->getTabs().size() == 1){
            emit this->displayTitleBarOnWindowRequested();
        }else{
            emit this->hideTitleBarOnWindowRequested();
        }
    });

    this->connect(this->groups.at(0), &TabGroup::windowTitleChanged, this, [=](QString title){
        emit this->titleChanged(title);
    });

    this->addWidget(this->groups.at(0));
}

TabGroup* TabManager::getGroup(int pos){
    return this->groups.at(pos);
}

TabGroup* TabManager::getCurrentGroup(){
    return this->groups.at(this->currentGroup);
}

void TabManager::windowSplitLeft(){
    this->getCurrentGroup()->splitLeft(0);
}

void TabManager::windowSplitRight(){
    this->getCurrentGroup()->splitRight(0);
}

void TabManager::windowCopyLink(){
    this->getCurrentGroup()->getTab(0)->copyUrl();
}

void TabManager::windowSearch(){
    this->getCurrentGroup()->getTab(0)->requestSearchDialog();
}

void TabManager::windowNextPage(){
    this->getCurrentGroup()->getTab(0)->requestNextPage();
}

void TabManager::windowPreviousPage(){
    this->getCurrentGroup()->getTab(0)->requestPreviousPage();
}

void TabManager::windowReload(){
    this->getCurrentGroup()->getTab(0)->requestReload();
}

TabManager::~TabManager(){
    for(TabGroup *group: this->groups){
        delete group;
    }
    this->groups.clear();
}