#include "SplitTabMenu.h"

SplitTabMenuAction::SplitTabMenuAction(QObject *parent): QWidgetAction(parent){
    this->defaultWidget = new QWidget();
    this->layout = new QHBoxLayout(this->defaultWidget);

    this->splitTabLeftButton = new IconButton(":/icons/split-left.png");
    this->splitTabRightButton = new IconButton(":/icons/split-right.png");

    this->layout->addWidget(this->splitTabLeftButton);
    this->layout->addWidget(this->splitTabRightButton);

    this->setDefaultWidget(this->defaultWidget);
}

SplitTabMenuAction::~SplitTabMenuAction(){
    delete this->defaultWidget;
    delete this->layout;
    delete this->splitTabLeftButton;
    delete this->splitTabRightButton;
}

SplitTabMenu::SplitTabMenu(QWidget *parent): IconButton(":/icons/split.png", parent){
    this->menu = new QMenu();
    this->menu->setStyleSheet(
        "QMenu{"
        "   border: none;"
        "   background-color: rgb(30, 30, 30);"
        "   border-radius: 10;"
        "}"
    );
    
    this->splitAction = new SplitTabMenuAction();
    this->menu->addAction(this->splitAction);

    this->connect(this, &IconButton::clicked, this, [=](){
        this->menu->popup(this->parentWidget()->mapToGlobal(this->geometry().bottomLeft()) + QPoint(-25, 5));
    });
}

SplitTabMenu::~SplitTabMenu(){
    delete this->menu;
    delete this->splitAction;
}