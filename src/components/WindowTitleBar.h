#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>

#include "AddressBox.h"
#include "IconButton.h"
#include "SplitTabMenu.h"
#include "../windows/PageSettingsDialog.h"
#include "../windows/DownloadManager.h"

#include "IconButton.h"

class WindowTitleBar : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *tabTitleBarLayout, *titleBarLayout, *windowButtonsLayout;
    IconButton *minimize, *maximize, *close;
    IconButton *reloadButton, *sideBarButton, *backButton, *forwardButton, *copyLinkButton, *siteSettingsButton, *groupSelectorButton, *downloadButton;
    SplitTabMenu *splitTabMenu;
    QWidget *tabTitleBar;
    AddressBox *addressBox;
    PageSettingsDialog *pageSettingsDialog;
public:
    WindowTitleBar(QWidget *parent = nullptr);
    QPushButton *minimizeButton();
    QPushButton *maximizeButton();
    QPushButton *closeButton();
    void setTitle(QString title);
    void setTitleBarVisible(bool visible);
    ~WindowTitleBar();
signals:
    void reloadRequested();
    void previousPageRequested();
    void nextPageRequested();
    void closeTabRequested();
    void splitTabLeftRequested();
    void splitTabRightRequested();
    void copyLinkRequested();
    void searchRequested();
    void toggleSideBarRequested();
    void showGroupBar();
    void showSiteSettingsRequested();
    void downloadDialogRequested();
};