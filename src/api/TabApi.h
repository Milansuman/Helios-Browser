#pragma once

#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <QList>
#include <QUrl>

/*
window.tabs.requestSplitTab(string url)
window.tabs.requestFlipTabs()
window.tabs.requestNewGroup(string url)
*/

class TabsApi: public QObject{
    Q_OBJECT
public:
    TabsApi(QObject *parent=nullptr);
    Q_INVOKABLE bool addTabs(QJsonDocument tabs);
    Q_INVOKABLE void requestSplitTab(QString url);
    Q_INVOKABLE void requestSplitTab();
    Q_INVOKABLE void requestFlipTabs();
    Q_INVOKABLE void requestNewGroup(QString url);
    ~TabsApi();
signals:
    void splitTabRequested(QUrl url);
    void splitTabHomeRequested();
    void splitTabFlipRequested();
    void newTabRequested(QUrl url);
    void addTabsRequested(QList<QList<QUrl>>);
};