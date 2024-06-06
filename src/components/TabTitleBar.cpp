#include "TabTitleBar.h"

TabTitleBar::TabTitleBar(QWidget *parent): QWidget(parent){
    this->setFixedHeight(30);
    this->layout = new QHBoxLayout(this);
    
    this->backButton = new IconButton(":/icons/chevron-left.png");
    this->forwardButton = new IconButton(":/icons/chevron-right.png");
    this->copyLinkButton = new IconButton(":/icons/link.png");

    this->addressBox = new AddressBox("search or enter link.");

    this->siteSettingsButton = new IconButton(":/icons/page-settings.png");
    this->splitTabMenu = new SplitTabMenu();
    this->closeButton = new IconButton(":/icons/tab-close.png");

    this->layout->addWidget(this->backButton);
    this->layout->addWidget(this->forwardButton);
    this->layout->addStretch();
    this->layout->addWidget(this->copyLinkButton);
    this->layout->addWidget(this->addressBox);
    this->layout->addWidget(this->siteSettingsButton);
    this->layout->addStretch();
    this->layout->addWidget(this->splitTabMenu);
    this->layout->addWidget(this->closeButton);
}

TabTitleBar::~TabTitleBar(){
    delete this->layout;
    delete this->backButton;
    delete this->forwardButton;
    delete this->copyLinkButton;
    delete this->addressBox;
    delete this->siteSettingsButton;
    delete this->closeButton;
}