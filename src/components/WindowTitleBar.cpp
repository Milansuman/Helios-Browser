#include "WindowTitleBar.h"

#include <QIcon>
#include <QApplication>
#include <QStyle>
#include <QPainter>

WindowTitleBar::WindowTitleBar(QWidget *parent) : QWidget(parent)
{
    this->setContentsMargins(0, 0, 0, 0);
    this->setFixedHeight(30);
    this->tabTitleBar = new QWidget();

    QSizePolicy retainWidth = this->tabTitleBar->sizePolicy();
    retainWidth.setRetainSizeWhenHidden(true);
    this->tabTitleBar->setSizePolicy(retainWidth);

    this->tabTitleBar->setContentsMargins(0, 0, 0, 0);
    this->tabTitleBarLayout = new QHBoxLayout(this->tabTitleBar);
    this->tabTitleBarLayout->setContentsMargins(15, 0, 15, 0);
    this->titleBarLayout = new QHBoxLayout(this);
    this->titleBarLayout->setContentsMargins(0, 0, 0, 0);
    this->titleBarLayout->setAlignment(Qt::AlignCenter);
    this->windowButtonsLayout = new QHBoxLayout();
    this->windowButtonsLayout->setAlignment(Qt::AlignCenter);
    this->windowButtonsLayout->setContentsMargins(0, 0, 0, 0);

    this->sideBarButton = new IconButton(":/icons/white/sidebar.png");
    this->groupSelectorButton = new IconButton(":/icons/white/group-selector.png");
    this->backButton = new IconButton(":/icons/white/chevron-left.png");
    this->forwardButton = new IconButton(":/icons/white/chevron-right.png");
    this->reloadButton = new IconButton(":/icons/white/reload.png");
    this->downloadButton = new IconButton(":/icons/white/download.png");
    this->copyLinkButton = new IconButton(":/icons/white/link.png");
    this->addressBox = new AddressBox("search or enter link.");
    this->siteSettingsButton = new IconButton(":/icons/white/page-settings.png");

    this->splitTabMenu = new SplitTabMenu();
    // this->downloadDialog = new DownloadDialog();
    this->splitTabMenu->setButtonIcon(":/icons/white/split.png");

    this->connect(this->sideBarButton, &IconButton::clicked, this, [=](){ 
        emit this->toggleSideBarRequested(); 
    });

    this->connect(this->addressBox, &AddressBox::searchRequested, this, [=](){ 
        emit this->searchRequested(); 
    });

    this->connect(this->backButton, &IconButton::clicked, this, [=](){ 
        emit this->previousPageRequested(); 
    });

    this->connect(this->forwardButton, &IconButton::clicked, this, [=](){ 
        emit this->nextPageRequested(); 
    });

    this->connect(this->reloadButton, &IconButton::clicked, this, [=](){ 
        emit this->reloadRequested(); 
    });

    this->connect(this->downloadButton, &IconButton::clicked, this, [=](){ 
        emit this->downloadDialogRequested();
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

    this->connect(this->siteSettingsButton, &IconButton::clicked, this, [=](){ 
        emit this->showSiteSettingsRequested(); 
    });

    this->connect(this->groupSelectorButton, &IconButton::clicked, this, [=](){ 
        emit this->showGroupBar(); 
    });

    this->minimize = new IconButton(":/icons/minimize.png");
    this->minimize->scale(13, 13);
    this->maximize = new IconButton(":/icons/maximize.png");
    this->maximize->scale(13, 13);
    this->close = new IconButton(":/icons/close.png");
    this->close->scale(13, 13);

    this->titleBarLayout->addWidget(this->sideBarButton);
    this->titleBarLayout->addWidget(this->groupSelectorButton);
    this->titleBarLayout->addWidget(this->downloadButton);
    this->tabTitleBarLayout->addWidget(this->backButton);
    this->tabTitleBarLayout->addWidget(this->forwardButton);
    this->tabTitleBarLayout->addWidget(this->reloadButton);
    this->tabTitleBarLayout->addStretch();
    this->tabTitleBarLayout->addWidget(this->copyLinkButton);
    this->tabTitleBarLayout->addWidget(this->addressBox);
    this->tabTitleBarLayout->addWidget(this->siteSettingsButton);
    this->tabTitleBarLayout->addStretch();
    this->tabTitleBarLayout->addWidget(this->splitTabMenu);

    this->windowButtonsLayout->addWidget(this->minimize);
    this->windowButtonsLayout->addWidget(this->maximize);
    this->windowButtonsLayout->addWidget(this->close);

    this->titleBarLayout->addWidget(this->tabTitleBar);
    this->titleBarLayout->addLayout(this->windowButtonsLayout);
}

QPushButton *WindowTitleBar::minimizeButton()
{
    return this->minimize;
}

QPushButton *WindowTitleBar::maximizeButton()
{
    return this->maximize;
}

QPushButton *WindowTitleBar::closeButton()
{
    return this->close;
}

void WindowTitleBar::setTitle(QString title)
{
    this->addressBox->setBlank(false);
    this->addressBox->setText(title);
}

void WindowTitleBar::setTitleBarVisible(bool visible)
{
    this->tabTitleBar->setVisible(visible);
}

WindowTitleBar::~WindowTitleBar()
{
    delete this->tabTitleBarLayout;
    delete this->windowButtonsLayout;
    delete this->titleBarLayout;
    delete this->minimize;
    delete this->maximize;
    delete this->close;
    delete this->sideBarButton;
    delete this->addressBox;
    delete this->copyLinkButton;
    delete this->siteSettingsButton;
    delete this->backButton;
    delete this->forwardButton;
    delete this->reloadButton;
    delete this->groupSelectorButton;
    delete this->downloadButton;
}
