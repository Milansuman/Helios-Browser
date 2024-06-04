#pragma once

#include <vector>
#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QWebEngineProfile>
#include "tab.h"
#include "searchDialog.h"

class TabGroup : public QWidget{
    Q_OBJECT
private:
    QWebEngineProfile *profile;
    std::vector<Tab*> tabs;
    Tab *mainTab; //set tab whenever there is only one tab in a group
    QHBoxLayout *splitScreenLayout;
    QSplitter *tabSplitter;
    void insertTab(Tab *tab, int pos);
    void handleTitleBars(int count);
public:
    TabGroup(QWebEngineProfile *profile, QWidget *parent=nullptr);
    //pass tab to find the position to insert at
    void splitLeft(Tab* tab);
    void splitRight(Tab* tab);
    Tab* getTab(int pos);
    Tab* getMainTab();
    ~TabGroup();
signals:
    void tabsChanged(int count);
    void reachedSingleTab(Tab* tab);
};