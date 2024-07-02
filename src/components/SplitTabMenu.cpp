#include "SplitTabMenu.h"

#include <QPainter>

SplitTabMenuAction::SplitTabMenuAction(QObject *parent): QWidgetAction(parent){
    this->defaultWidget = new QWidget();
    this->layout = new QHBoxLayout(this->defaultWidget);

    this->splitTabLeftButton = new IconButton(":/icons/white/split-left.png");
    this->splitTabRightButton = new IconButton(":/icons/white/split-right.png");
    this->splitTabFlipButton = new IconButton(":/icons/white/download.png");

    this->connect(this->splitTabLeftButton, &IconButton::clicked, this, [=](){
        emit this->splitTabLeftRequested();
    });

    this->connect(this->splitTabRightButton, &IconButton::clicked, this, [=](){
        emit this->splitTabRightRequested();
    });

    this->connect(this->splitTabFlipButton, &IconButton::clicked, this, [=](){
        emit this->splitTabFlipRequested();
    });

    this->layout->addWidget(this->splitTabLeftButton);
    this->layout->addWidget(this->splitTabRightButton);
    this->layout->addWidget(this->splitTabFlipButton);

    this->setDefaultWidget(this->defaultWidget);
}

SplitTabMenuAction::~SplitTabMenuAction(){
    delete this->splitTabLeftButton;
    delete this->splitTabRightButton;
    delete this->layout;
    delete this->defaultWidget;
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

    this->connect(this->splitAction, &SplitTabMenuAction::splitTabLeftRequested, this, [=](){
        emit this->splitTabLeftRequested();
        this->menu->hide();
    });

    this->connect(this->splitAction, &SplitTabMenuAction::splitTabRightRequested, this, [=](){
        emit this->splitTabRightRequested();
        this->menu->hide();
    });

    this->connect(this->splitAction, &SplitTabMenuAction::splitTabFlipRequested, this, [=](){
        emit this->splitTabFlipRequested();
        this->menu->hide();
    });
}

SplitTabMenu::~SplitTabMenu(){
    delete this->menu;
    delete this->splitAction;
}