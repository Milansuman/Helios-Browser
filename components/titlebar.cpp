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
    //=======================TITLE BAR=======================================
    QHBoxLayout *titlebarLayout = new QHBoxLayout;

    //=======================ADDRESS BAR=====================================
    QHBoxLayout *addressbarLayout = new QHBoxLayout;

    AddressBox *search = new AddressBox("search or enter link");
    addressbarLayout->addWidget(search, 0, Qt::AlignCenter);

    //=======================TITLE BAR BUTTON=================================
    QHBoxLayout *titlebarButtonsLayout = new QHBoxLayout;

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