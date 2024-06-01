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
    ~TabGroup();
signals:
    void tabsChanged(int count);
    void reachedSingleTab(Tab* tab);
};