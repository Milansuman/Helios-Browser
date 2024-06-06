#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

#include "AddressBox.h"
#include "IconButton.h"
#include "SplitTabMenu.h"

class WindowTitleBar : public QWidget{
    Q_OBJECT
private:
    QHBoxLayout *tabTitleBarLayout, *titleBarLayout, *windowButtonsLayout;
    QPushButton *minimize, *maximize, *close;
    IconButton *reloadButton, *sideBarButton, *backButton, *forwardButton, *copyLinkButton, *siteSettingsButton;
    SplitTabMenu *splitTabMenu;
    QWidget *tabTitleBar;
    AddressBox *addressBox;
public:
    WindowTitleBar(QWidget *parent=nullptr);
    QPushButton* minimizeButton();
    QPushButton* maximizeButton();
    QPushButton* closeButton();
    void setTitle(QString title);
    ~WindowTitleBar();
signals:
    void reloadRequested();
    void previousPageRequested();
    void nextPageRequested();
    void closeTabRequested();
    void splitTabLeftRequested();
    void splitTabRightRequested();
    void copyLinkRequested();
};