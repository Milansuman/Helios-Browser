#include "TabApi.h"

#include <QJsonArray>

TabsApi::TabsApi(QObject *parent): QObject(parent){}

bool TabsApi::addTabs(QJsonDocument tabs){
    if(tabs.isArray()){
        QList<QList<QUrl>> tabsList;
        for(QJsonValueRef group: tabs.array()){
            if(group.isArray()){
                QList<QUrl> urls;

                for(QJsonValueRef url: group.toArray()){
                    urls.append(QUrl(url.toString()));
                }
                tabsList.append(urls);
            }else{
                return false;
            }
        }
        emit this->addTabsRequested(tabsList);
    }else{
        return false;
    }
    return true;
}

void TabsApi::requestSplitTab(QString url){
    emit this->splitTabRequested(QUrl(url));
}

void TabsApi::requestSplitTab(){
    emit this->splitTabHomeRequested();
}

void TabsApi::requestFlipTabs(){
    emit this->splitTabFlipRequested();
}

void TabsApi::requestNewGroup(QString url){
    emit this->newTabRequested(QUrl(url));
}

TabsApi::~TabsApi() = default;