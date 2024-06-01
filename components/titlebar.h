#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QHBoxLayout>
#include "tabTitleBar.h"
#include "titlebarbuttons.h"
#include "../browserwindow.h"
#include "searchDialog.h"

class TitleBar : public QWidget {
    Q_OBJECT
private:
    TabTitleBar *titlebar;
    QHBoxLayout *titlebarLayout;
    QHBoxLayout *titlebarButtonsLayout;
    TitleBarButtons::MinimizeButton *minimizeButton;
    TitleBarButtons::MaximizeButton *maximizeButton;
    TitleBarButtons::CloseButton *closeButton;
public:
    TitleBar(BrowserWindow *window, QWidget *parent = nullptr);
    void addTabTitleBar(TabTitleBar* titlebar);
    void removeTabTitleBar();
    ~TitleBar();
};