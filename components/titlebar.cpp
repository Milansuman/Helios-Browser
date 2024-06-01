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

    //=======================TITLE BAR BUTTON=================================
    titlebarButtonsLayout = new QHBoxLayout();

    titlebarButtonsLayout->setSpacing(3);
    titlebarButtonsLayout->setContentsMargins(0, 5, 0, 0);

    minimizeButton = new TitleBarButtons::MinimizeButton(window);
    maximizeButton = new TitleBarButtons::MaximizeButton(window);
    closeButton = new TitleBarButtons::CloseButton(window);
    titlebarButtonsLayout->addWidget(minimizeButton);
    titlebarButtonsLayout->addWidget(maximizeButton);
    titlebarButtonsLayout->addWidget(closeButton);

    //=======================SETTING UP LAYOUTS===============================
    this->titlebarLayout = new QHBoxLayout();
    this->titlebarLayout->setContentsMargins(0, 0, 0, 0);
    
    titlebarLayout->addLayout(titlebarButtonsLayout);
    this->setLayout(titlebarLayout);
}

void TitleBar::addTabTitleBar(TabTitleBar *titlebar){
    this->titlebar = titlebar;
    this->titlebar->setVisible(true);
    this->titlebarLayout->insertWidget(0, this->titlebar);
}

void TitleBar::removeTabTitleBar(){
    this->titlebar = nullptr;
    this->titlebarLayout->removeWidget(this->titlebar);
}

TitleBar::~TitleBar(){
    delete this->titlebarLayout;
    delete this->titlebar;
    delete this->titlebarButtonsLayout;
    delete this->minimizeButton;
    delete this->maximizeButton;
    delete this->closeButton;
} 