#pragma once

#include <vector>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QWidget>
#include <QWebEngineProfile>

#include "tabTitleBar.h"
#include "tabGroup.h"
#include "searchDialog.h"

class TabManager : public QStackedWidget {
    Q_OBJECT
private:
    std::vector<TabGroup*> groups;
    QWebEngineProfile *profile;
public:
    TabManager(QWidget *parent=nullptr);
    TabGroup* getTabGroup(int pos);
    ~TabManager();
signals:
    void embedTabTitlebar(TabTitleBar* titlebar);
    void removeTabTitleBar();
};