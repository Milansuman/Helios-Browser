#pragma once

#include <vector>
#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include "tab.h"

class TabGroup : public QWidget{
    Q_OBJECT
private:
    QHBoxLayout *splitScreenLayout;
    QSplitter *tabSplitter;
    void insertTab(Tab *tab, int pos);
    void handleTitleBars(int count);
public:
    std::vector<Tab*> tabs;
    TabGroup(QWidget *parent=nullptr);
    //pass tab to find the position to insert at
    void splitLeft(Tab* tab);
    void splitRight(Tab* tab);
    ~TabGroup();
signals:
    void splitTabs();
    void tabsChanged(int count);
};