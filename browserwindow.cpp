#include "browserwindow.h"
#include "components/titlebarbuttons.h"
#include <QMainWindow>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>

BrowserWindow::BrowserWindow(QWidget *parent, double width, double height): QMainWindow(parent){
    this->resize(width, height);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    //Implement Outer UI
    QWidget *centralWidget = new QWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *titlebarLayout = new QHBoxLayout;

    //=======================ADDRESS BAR=====================================
    QHBoxLayout *addressbarLayout = new QHBoxLayout;
    

    //=======================TITLE BAR BUTTON=================================
    QHBoxLayout *titlebarButtonsLayout = new QHBoxLayout;

    TitleBarButtons::MinimizeButton *minimizeButton = new TitleBarButtons::MinimizeButton(this);
    TitleBarButtons::MaximizeButton *maximizeButton = new TitleBarButtons::MaximizeButton(this);
    TitleBarButtons::CloseButton *closeButton = new TitleBarButtons::CloseButton(this);
    titlebarButtonsLayout->addWidget(minimizeButton);
    titlebarButtonsLayout->addWidget(maximizeButton);
    titlebarButtonsLayout->addWidget(closeButton);

    //=======================SETTING UP LAYOUTS===============================
    titlebarLayout->addLayout(addressbarLayout);
    titlebarLayout->addLayout(titlebarButtonsLayout);
    mainLayout->addLayout(titlebarLayout);

    this->setCentralWidget(centralWidget);
}

void BrowserWindow::paintEvent(QPaintEvent *event){
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //setting color and rounded corners
    painter.setBrush(QBrush(QColor(26, 29, 34)));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 10, 10);
}

BrowserWindow::~BrowserWindow(){
}