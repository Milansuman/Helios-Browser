#include "TabTitleBar.h"

TabTitleBar::TabTitleBar(QWidget *parent): QWidget(parent){
    this->setFixedHeight(30);
    this->layout = new QHBoxLayout(this);
    this->layout->setAlignment(Qt::AlignCenter);
    
    this->backButton = new IconButton(":/icons/chevron-left.png");
    this->forwardButton = new IconButton(":/icons/chevron-right.png");
    this->reloadButton = new IconButton(":/icons/reload.png");
    this->copyLinkButton = new IconButton(":/icons/link.png");

    this->addressBox = new AddressBox("search or enter link.");
    this->addressBox->setBlank(false);

    this->siteSettingsButton = new IconButton(":/icons/page-settings.png");
    this->splitTabMenu = new SplitTabMenu();
    this->closeButton = new IconButton(":/icons/tab-close.png");

    this->connect(this->addressBox, &AddressBox::searchRequested, this, [=](){
        emit this->searchRequested();
    });

    this->connect(this->reloadButton, &IconButton::clicked, this, [=](){
        emit this->reloadRequested();
    });

    this->connect(this->backButton, &IconButton::clicked, this, [=](){
        emit this->previousPageRequested();
    });

    this->connect(this->forwardButton, &IconButton::clicked, this, [=](){
        emit this->nextPageRequested();
    });

    this->connect(this->copyLinkButton, &IconButton::clicked, this, [=](){
        emit this->copyLinkRequested();
    });

    this->connect(this->splitTabMenu, &SplitTabMenu::splitTabLeftRequested, this, [=](){
        emit this->splitTabLeftRequested();
    });

    this->connect(this->splitTabMenu, &SplitTabMenu::splitTabRightRequested, this, [=](){
        emit this->splitTabRightRequested();
    });

    this->connect(this->closeButton, &IconButton::clicked, this, [=](){
        emit this->closeTabRequested();
    });

    this->layout->addWidget(this->backButton);
    this->layout->addWidget(this->forwardButton);
    this->layout->addWidget(this->reloadButton);
    this->layout->addStretch();
    this->layout->addWidget(this->copyLinkButton);
    this->layout->addWidget(this->addressBox);
    this->layout->addWidget(this->siteSettingsButton);
    this->layout->addStretch();
    this->layout->addWidget(this->splitTabMenu);
    this->layout->addWidget(this->closeButton);
}

void TabTitleBar::setTitle(QString title){
    this->addressBox->setText(title);
}

TabTitleBar::~TabTitleBar(){
    delete this->layout;
    delete this->backButton;
    delete this->forwardButton;
    delete this->copyLinkButton;
    delete this->addressBox;
    delete this->siteSettingsButton;
    delete this->closeButton;
    delete this->reloadButton;
}