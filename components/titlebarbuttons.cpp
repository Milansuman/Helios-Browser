#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include "titlebarbuttons.h"

TitleBarButtons::CloseButton::CloseButton(BrowserWindow *window, QWidget *parent) : QWidget(parent){
    this->window = window;
    this->setFixedSize(15, 15);
}

void TitleBarButtons::CloseButton::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);

    painter.drawEllipse(0, 0, 15, 15);
}

void TitleBarButtons::CloseButton::mousePressEvent(QMouseEvent *event){
    this->window->close();
}

TitleBarButtons::CloseButton::~CloseButton(){}

TitleBarButtons::MinimizeButton::MinimizeButton(BrowserWindow *window, QWidget *parent) : QWidget(parent){
    this->window = window;
    this->setFixedSize(15, 15);
}

void TitleBarButtons::MinimizeButton::mousePressEvent(QMouseEvent *event){
    this->window->showMinimized();
}

void TitleBarButtons::MinimizeButton::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::green);

    painter.drawEllipse(0, 0, 15, 15);
}

TitleBarButtons::MinimizeButton::~MinimizeButton(){}

TitleBarButtons::MaximizeButton::MaximizeButton(BrowserWindow *window, QWidget *parent) : QWidget(parent){
    this->window = window;
    this->setFixedSize(15, 15);
}

void TitleBarButtons::MaximizeButton::mousePressEvent(QMouseEvent *event){
    if(this->window->maximized){
        this->window->showNormal();
        this->window->maximized = false;
    }else{
        this->window->showMaximized();
        this->window->maximized = true;
    }
}

void TitleBarButtons::MaximizeButton::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::yellow);

    painter.drawEllipse(0, 0, 15, 15);
}

TitleBarButtons::MaximizeButton::~MaximizeButton(){}