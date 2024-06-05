#include "WindowTitleBar.h"

#include <QIcon>
#include <QApplication>
#include <QStyle>

WindowTitleBar::WindowTitleBar(QWidget *parent): QWidget(parent){
    this->tabTitleBarLayout = new QHBoxLayout(this);
    this->windowButtonsLayout = new QHBoxLayout();

    //Creating window titlebar buttons
    QStyle *style = qApp->style();
    QIcon minimizeIcon = style->standardIcon(QStyle::SP_TitleBarMinButton);
    QIcon maximizeIcon = style->standardIcon(QStyle::SP_TitleBarMaxButton);
    QIcon closeIcon = style->standardIcon(QStyle::SP_TitleBarCloseButton);

    this->minimize = new QPushButton();
    this->minimize->setIcon(minimizeIcon);
    this->minimize->setStyleSheet(
        "QPushButton{"
        "   background: transparent;"
        "   border: none;"
        "}"
    );

    this->maximize = new QPushButton();
    this->maximize->setIcon(maximizeIcon);
    this->maximize->setStyleSheet(
        "QPushButton{"
        "   background: transparent;"
        "   border: none;"
        "}"
    );

    this->close = new QPushButton();
    this->close->setIcon(closeIcon);
    this->close->setStyleSheet(
        "QPushButton{"
        "   background: transparent;"
        "   border: none;"
        "}"
    );

    this->windowButtonsLayout->addWidget(this->minimize);
    this->windowButtonsLayout->addWidget(this->maximize);
    this->windowButtonsLayout->addWidget(this->close);

    this->tabTitleBarLayout->addLayout(this->windowButtonsLayout);
}

QPushButton* WindowTitleBar::minimizeButton(){
    return this->minimize;
}

QPushButton* WindowTitleBar::maximizeButton(){
    return this->maximize;
}

QPushButton* WindowTitleBar::closeButton(){
    return this->close;
}

WindowTitleBar::~WindowTitleBar(){
    delete this->tabTitleBarLayout;
    delete this->windowButtonsLayout;
    delete this->minimize;
    delete this->maximize;
    delete this->close;
}