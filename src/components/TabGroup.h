#pragma once

#include <QWebEngineProfile>
#include <QSplitter>
#include <QUrl>
#include <vector>

#include "Tab.h"

class TabGroup : public QSplitter{
    Q_OBJECT
private:
    std::vector<Tab*> tabs;
    Tab* activeTab;
    QWebEngineProfile *profile;
    int findTab(Tab *tab);
public:
    TabGroup(QWebEngineProfile *profile, QWidget *parent=nullptr);
    void splitLeft(int pos);
    void splitRight(int pos);
    void removeTab(int pos);
    void openDevTools();
    std::vector<Tab*> getTabs();
    Tab* getTab(int pos);
    ~TabGroup();
signals:
    void tabsChanged();
    void tabIconChanged();
    void windowTitleChanged(QString title);
    void newTabRequested(QUrl url);
    void newWindowRequested(QUrl url);
    void searchRequested(int pos);
    void navigationRequested(QString title, QUrl url);
};