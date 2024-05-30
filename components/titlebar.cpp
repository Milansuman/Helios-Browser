#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "titlebar.h"
#include "addressbox.h"
#include "../browserwindow.h"
#include "titlebarbuttons.h"

TitleBar::TitleBar(BrowserWindow *window, QWidget *parent): 
    QWidget(parent)
{
    this->setMaximumHeight(25);
    //=======================TITLE BAR=======================================
    QHBoxLayout *titlebarLayout = new QHBoxLayout;

    //=======================ADDRESS BAR=====================================
    QHBoxLayout *addressbarLayout = new QHBoxLayout;

    AddressBox *search = new AddressBox("search or enter link.", window);
    addressbarLayout->addWidget(search, 0, Qt::AlignCenter);

    //=======================TITLE BAR BUTTON=================================
    QHBoxLayout *titlebarButtonsLayout = new QHBoxLayout;

    titlebarButtonsLayout->setSpacing(3);
    titlebarButtonsLayout->setContentsMargins(0, 5, 0, 0);

    TitleBarButtons::MinimizeButton *minimizeButton = new TitleBarButtons::MinimizeButton(window);
    TitleBarButtons::MaximizeButton *maximizeButton = new TitleBarButtons::MaximizeButton(window);
    TitleBarButtons::CloseButton *closeButton = new TitleBarButtons::CloseButton(window);
    titlebarButtonsLayout->addWidget(minimizeButton);
    titlebarButtonsLayout->addWidget(maximizeButton);
    titlebarButtonsLayout->addWidget(closeButton);

    //=======================SETTING UP LAYOUTS===============================
    titlebarLayout->addLayout(addressbarLayout);
    titlebarLayout->addLayout(titlebarButtonsLayout);
    this->setLayout(titlebarLayout);
}

TitleBar::~TitleBar(){} 