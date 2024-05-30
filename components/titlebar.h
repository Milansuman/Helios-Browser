#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "addressbox.h"
#include "titlebarbuttons.h"
#include "../browserwindow.h"

class TitleBar : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout *titlebarLayout;
    QHBoxLayout *addressbarLayout;
    AddressBox *search;
    QHBoxLayout *titlebarButtonsLayout;
    TitleBarButtons::MinimizeButton *minimizeButton;
    TitleBarButtons::MaximizeButton *maximizeButton;
    TitleBarButtons::CloseButton *closeButton;
public:
    TitleBar(BrowserWindow *window, QWidget *parent = nullptr);
    void setTitle(QString title);
    ~TitleBar();
};