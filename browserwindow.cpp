#include "browserwindow.h"
#include "components/titlebarbuttons.h"
#include "components/addressbox.h"
#include <QMainWindow>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>


BrowserWindow::BrowserWindow(QWidget *parent, double width, double height): QMainWindow(parent){
    this->resize(width, height);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    //Implement Outer UI
    QWidget *centralWidget = new QWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    //=======================TITLE BAR=======================================
    QHBoxLayout *titlebarLayout = new QHBoxLayout;

    //=======================ADDRESS BAR=====================================
    QHBoxLayout *addressbarLayout = new QHBoxLayout;

    AddressBox *search = new AddressBox("search or enter link");
    addressbarLayout->addWidget(search, 0, Qt::AlignCenter);

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
    mainLayout->addStretch();

    this->setCentralWidget(centralWidget);
}

void BrowserWindow::paintEvent(QPaintEvent *event){
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //setting color and rounded corners
    painter.setBrush(QBrush(QColor(26, 29, 34)));
    painter.setPen(Qt::NoPen);
    if(this->maximized){
        painter.drawRect(rect());
    }else{
        painter.drawRoundedRect(rect(), 10, 10);
    }
}

void BrowserWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && this->isEdgePosition(event->position())){
        this->resizing = true;
    }
    QMainWindow::mousePressEvent(event);
}

void BrowserWindow::mouseMoveEvent(QMouseEvent *event){
    if(this->isEdgePosition(event->position())){
        this->setCursor(Qt::SizeAllCursor);
        qDebug() << event->position();
    }
    qDebug() << event->position();
    QMainWindow::mouseMoveEvent(event);
}

void BrowserWindow::mouseReleaseEvent(QMouseEvent *event){
    QMainWindow::mouseReleaseEvent(event);
}

bool BrowserWindow::isEdgePosition(QPointF position){
    if(position.x() <= this->width() && position.y() == 0){
        return true;
    }else if(position.y() <= this->height() && position.x() == 0){
        return true;
    }else if(position.x() <= this->width() && position.y() == this->height()){
        return true;
    }else if(position.y() <= this->height() && position.x() == this->width()){
        return true;
    }
    return false;
}

BrowserWindow::~BrowserWindow(){}