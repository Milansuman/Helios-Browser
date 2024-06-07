#pragma once

#include <QWebEngineProfile>
#include <QSplitter>
#include <vector>

#include "Tab.h"

class TabGroup : public QSplitter{
    Q_OBJECT
private:
    std::vector<Tab*> tabs;
    QWebEngineProfile *profile;

    int findTab(Tab *tab);
public:
    TabGroup(QWebEngineProfile *profile, QWidget *parent=nullptr);
    void splitLeft(int pos);
    void splitRight(int pos);
    void removeTab(int pos);
    std::vector<Tab*> getTabs();
    ~TabGroup();
signals:
    void tabsChanged();
};