#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include "titlebarbuttons.h"

TitleBarButtons::CloseButton::CloseButton(BrowserWindow *window, QWidget *parent) : QWidget(parent){
    this->window = window;
    this->setFixedSize(12, 12);
}

void TitleBarButtons::CloseButton::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(255, 93, 73)));

    painter.drawEllipse(0, 0, this->width(), this->height());
}

void TitleBarButtons::CloseButton::mousePressEvent(QMouseEvent *event){
    this->window->close();
}

TitleBarButtons::CloseButton::~CloseButton(){}

TitleBarButtons::MinimizeButton::MinimizeButton(BrowserWindow *window, QWidget *parent) : QWidget(parent){
    this->window = window;
    this->setFixedSize(12, 12);
}

void TitleBarButtons::MinimizeButton::mousePressEvent(QMouseEvent *event){
    this->window->showMinimized();
}

void TitleBarButtons::MinimizeButton::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(34, 203, 62)));

    painter.drawEllipse(0, 0, this->width(), this->height());
}

TitleBarButtons::MinimizeButton::~MinimizeButton(){}

TitleBarButtons::MaximizeButton::MaximizeButton(BrowserWindow *window, QWidget *parent) : QWidget(parent){
    this->window = window;
    this->setFixedSize(12, 12);
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
    painter.setBrush(QBrush(QColor(255, 188, 47)));

    painter.drawEllipse(0, 0, this->width(), this->height());
}

TitleBarButtons::MaximizeButton::~MaximizeButton(){}