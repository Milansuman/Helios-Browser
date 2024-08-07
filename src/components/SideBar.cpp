#include "SideBar.h"

SideBar::SideBar(QWidget *parent): QWidget(parent){
    this->setVisible(false);
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(5,10,10,5);
    this->layout->setSpacing(10);

    this->programmingBookMarkGroup = new BookMarkGroup("Programming Tools", QList<struct Bookmark>({
        {
            QPixmap(":/icons/white/code-solid.png").scaledToHeight(25),
            "VSCode",
            QUrl("https://vscode.dev")
        },
        {
            QPixmap(":/icons/white/github-brands-solid.png").scaledToHeight(25),
            "Github",
            QUrl("https://github.com")
        },
        {
            QPixmap(":/icons/white/codepen-brands-solid.png").scaledToWidth(25),
            "Codepen",
            QUrl("https://codepen.io")
        }
    }));

    this->connect(this->programmingBookMarkGroup, &BookMarkGroup::loadRequested, this, [=](QUrl url){
        emit this->openURL(url);
    });

    this->designBookMarkGroup = new BookMarkGroup("Design Tools", QList<struct Bookmark>({
        {
            QPixmap(":/icons/white/figma-brands-solid.png").scaledToHeight(25),
            "Figma",
            QUrl("https://figma.com")
        },
        {
            QPixmap(":/icons/white/dribbble-brands-solid.png").scaledToHeight(25),
            "Dribbble",
            QUrl("https://dribbble.com")
        },
        {
            QPixmap(":/icons/white/behance-brands-solid.png").scaledToHeight(25),
            "Behance",
            QUrl("https://www.behance.net")
        }
    }));

    this->connect(this->designBookMarkGroup, &BookMarkGroup::loadRequested, this, [=](QUrl url){
        emit this->openURL(url);
    });

    this->socialBookMarkGroup = new BookMarkGroup("Social Media", QList<struct Bookmark>({
        {
            QPixmap(":/icons/white/youtube-brands-solid.png").scaledToHeight(25),
            "Youtube",
            QUrl("https://youtube.com")
        },
        {
            QPixmap(":/icons/white/telegram-brands-solid.png").scaledToHeight(25),
            "Telegram",
            QUrl("https://web.telegram.org")
        },
        {
            QPixmap(":/icons/white/twitter-brands-solid.png").scaledToHeight(25),
            "Twitter (X)",
            QUrl("https://x.com")
        },
        {
            QPixmap(":/icons/white/instagram-brands-solid.png").scaledToHeight(25),
            "Instagram",
            QUrl("https://www.instagram.com")
        },
        {
            QPixmap(":/icons/white/linkedin-brands-solid.png").scaledToHeight(25),
            "LinkedIn",
            QUrl("https://www.linkedin.com")
        }
    }));

    this->connect(this->socialBookMarkGroup, &BookMarkGroup::loadRequested, this, [=](QUrl url){
        emit this->openURL(url);
    });

    this->bottomRowLayout = new QHBoxLayout();
    this->bottomRowLayout->setContentsMargins(0,0,0,0);
    this->bottomRowLayout->setSpacing(10);

    this->gridViewButton = new IconButton(":/icons/grid.png");
    this->gridViewButton->setStyleSheet(
        "QPushButton{"
        "   background-color: rgba(0,0,0,0.5);"
        "   border-radius: 5px;"
        "}"
    );
    this->gridViewButton->setFixedSize(60, 40);
    
    this->newGroupButton = new IconButton(":/icons/plus.png");
    this->newGroupButton->setStyleSheet(
        "QPushButton{"
        "   background-color: rgba(0,0,0,0.5);"
        "   border-radius: 5px;"
        "}"
    );
    this->newGroupButton->setFixedSize(60, 40);

    this->connect(this->newGroupButton, &IconButton::clicked, this, [=](){
        emit this->newGroupRequested();
    });

    this->bottomRowLayout->addWidget(this->gridViewButton);
    this->bottomRowLayout->addWidget(this->newGroupButton);

    this->layout->addWidget(this->programmingBookMarkGroup);
    this->layout->addWidget(this->designBookMarkGroup);
    this->layout->addWidget(this->socialBookMarkGroup);
    this->layout->addStretch();
    this->layout->addLayout(this->bottomRowLayout);
}

SideBar::~SideBar(){
    delete this->gridViewButton;
    delete this->newGroupButton;
    delete this->bottomRowLayout;
    delete this->layout;
}