#include "SideBar.h"

SideBar::SideBar(QWidget *parent): QWidget(parent){
    this->setVisible(false);
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(5,10,10,5);
    this->bottomRowLayout = new QHBoxLayout();
    this->bottomRowLayout->setContentsMargins(0,0,0,0);
    this->bottomRowLayout->setSpacing(10);

    this->gridViewButton = new IconButton(":/icons/grid.png");
    this->gridViewButton->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(0,0,0);"
        "   border-radius: 5px;"
        "}"
    );
    this->gridViewButton->setFixedSize(60, 40);
    
    this->newGroupButton = new IconButton(":/icons/plus.png");
    this->newGroupButton->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(0,0,0);"
        "   border-radius: 5px;"
        "}"
    );
    this->newGroupButton->setFixedSize(60, 40);

    this->bottomRowLayout->addWidget(this->gridViewButton);
    this->bottomRowLayout->addWidget(this->newGroupButton);

    this->layout->addStretch();
    this->layout->addLayout(this->bottomRowLayout);
}

SideBar::~SideBar(){
    delete this->gridViewButton;
    delete this->newGroupButton;
    delete this->bottomRowLayout;
    delete this->layout;
}