#include "TabApi.h"

TabsApi::TabsApi(QObject *parent): QObject(parent){}

void TabsApi::requestSplitTab(QString url){
    emit this->splitTabRequested(QUrl(url));
}

void TabsApi::requestFlipTabs(){
    emit this->splitTabFlipRequested();
}

void TabsApi::requestNewGroup(QString url){
    emit this->newTabRequested(QUrl(url));
}

TabsApi::~TabsApi() = default;