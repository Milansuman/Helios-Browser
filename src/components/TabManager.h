#pragma once

#include <vector>
#include <QStackedWidget>
#include <QWebEngineProfile>

#include "TabGroup.h"
#include "Tab.h"
#include "../windows/GroupSelectorDialog.h"

class TabManager : public QStackedWidget{
    Q_OBJECT
private:
    std::vector<TabGroup*> groups;
    int currentGroup;
    QWebEngineProfile *profile;
    GroupSelectorDialog *groupSelectorDialog;
public:
    TabManager(QWidget *parent=nullptr);
    TabGroup* getGroup(int pos);
    TabGroup* getCurrentGroup();
    void windowSplitLeft();
    void windowSplitRight();
    void windowSearch();
    void windowCopyLink();
    void windowPreviousPage();
    void windowNextPage();
    void windowReload();
    void windowShowGroups();
    ~TabManager();
signals:
    void displayTitleBarOnWindowRequested();
    void hideTitleBarOnWindowRequested();
    void titleChanged(QString title);
};