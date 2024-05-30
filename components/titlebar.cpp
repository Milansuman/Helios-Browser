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
    titlebarLayout = new QHBoxLayout;

    //=======================ADDRESS BAR=====================================
    addressbarLayout = new QHBoxLayout;

    search = new AddressBox("search or enter link.", window);
    addressbarLayout->addWidget(search, 0, Qt::AlignCenter);

    //=======================TITLE BAR BUTTON=================================
    titlebarButtonsLayout = new QHBoxLayout;

    titlebarButtonsLayout->setSpacing(3);
    titlebarButtonsLayout->setContentsMargins(0, 5, 0, 0);

    minimizeButton = new TitleBarButtons::MinimizeButton(window);
    maximizeButton = new TitleBarButtons::MaximizeButton(window);
    closeButton = new TitleBarButtons::CloseButton(window);
    titlebarButtonsLayout->addWidget(minimizeButton);
    titlebarButtonsLayout->addWidget(maximizeButton);
    titlebarButtonsLayout->addWidget(closeButton);

    //=======================SETTING UP LAYOUTS===============================
    titlebarLayout->addLayout(addressbarLayout);
    titlebarLayout->addLayout(titlebarButtonsLayout);
    this->setLayout(titlebarLayout);
}

void TitleBar::setTitle(QString title){
    this->search->setText(title);
    this->search->setBlank(false);
}

TitleBar::~TitleBar(){
    delete this->titlebarLayout;
    delete this->addressbarLayout;
    delete this->search;
    delete this->titlebarButtonsLayout;
    delete this->minimizeButton;
    delete this->maximizeButton;
    delete this->closeButton;
} 