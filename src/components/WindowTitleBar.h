#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

#include "AddressBox.h"

class WindowTitleBar : public QWidget{
    Q_OBJECT
private:
    QHBoxLayout *tabTitleBarLayout, *titleBarLayout, *windowButtonsLayout;
    QPushButton *minimize, *maximize, *close;
    QPushButton *sideBarButton;
    QWidget *tabTitleBar;
    AddressBox *addressBox;
public:
    WindowTitleBar(QWidget *parent=nullptr);
    QPushButton* minimizeButton();
    QPushButton* maximizeButton();
    QPushButton* closeButton();
    ~WindowTitleBar();
};