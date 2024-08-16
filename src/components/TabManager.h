#pragma once

#include <vector>
#include <QStackedWidget>
#include <QWebEngineProfile>
#include <QList>
#include <QUrl>
#include <QDateTime>

#include "TabGroup.h"
#include "Tab.h"
#include "../windows/GroupSelectorDialog.h"
#include "../windows/DownloadManager.h"

typedef struct History{
    QString title;
    QUrl url;
    QDateTime timestamp;
} History;

class TabManager : public QStackedWidget
{
    Q_OBJECT
private:
    std::vector<TabGroup *> groups;
    QList<QWebEngineDownloadRequest*> downloadRequests;
    QList<History> history;

    int currentGroup;
    QWebEngineProfile *profile;
    GroupSelectorDialog *groupSelectorDialog;
    DownloadManager *downloadManager;

public:
    TabManager(QWidget *parent = nullptr);
    TabGroup *getGroup(int pos);
    TabGroup *getCurrentGroup();
    void setInitialUrl(QUrl url);
    void addGroup();
    void addGroup(QUrl url);
    void closeGroup(int pos);
    void windowSplitLeft();
    void windowSplitRight();
    void windowSearch();
    void windowCopyLink();
    void windowPreviousPage();
    void windowNextPage();
    void windowReload();
    void windowShowGroups();
    void windowShowSiteSettings();
    void windowLoadBulk(QList<QList<QUrl>> tabsList);
    void windowShowDownloads();
    void windowLoadTab(int group, int tab, QString url);
    ~TabManager();
signals:
    void displayTitleBarOnWindowRequested();
    void hideTitleBarOnWindowRequested();
    void titleChanged(QString title);
    void newWindowRequested(QUrl url);
    void searchRequested(int pos, int group);
    void downloadsChanged(int downloads);
};