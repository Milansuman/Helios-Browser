#include "browserwindow.h"
#include "components/titlebarbuttons.h"
#include "components/addressbox.h"
#include <QMainWindow>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

BrowserWindow::BrowserWindow(QWidget*parent, double width, double height): QWidget(parent), resizing(false){
    this->resize(width, height);
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

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
}

void BrowserWindow::paintEvent(QPaintEvent *event){
    QWidget::paintEvent(event);
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
        this->showNormal();
        this->resizing = true;
        this->maximized = false;
        this->originalGeometry = this->geometry();
        this->lastMousePosition = event->globalPosition();
        this->currentEdgePosition = this->edgePosition(event->position());
    }
    QWidget::mousePressEvent(event);
}

void BrowserWindow::mouseMoveEvent(QMouseEvent *event){
    switch(this->edgePosition(event->position())){
        case WindowBoundary::TOP:
        case WindowBoundary::BOTTOM:
            this->setCursor(Qt::SizeVerCursor);
            break;
        case WindowBoundary::RIGHT:
        case WindowBoundary::LEFT:
            this->setCursor(Qt::SizeHorCursor);
            break;
        case WindowBoundary::TOP_LEFT:
        case WindowBoundary::BOTTOM_RIGHT:
            this->setCursor(Qt::SizeFDiagCursor);
            break;
        case WindowBoundary::TOP_RIGHT:
        case WindowBoundary::BOTTOM_LEFT:
            this->setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            this->setCursor(Qt::ArrowCursor);
    }

    if(this->resizing){
        QPointF delta = event->globalPosition() - lastMousePosition;
        QRect newGeometry = originalGeometry;

        switch(this->currentEdgePosition){
        case WindowBoundary::TOP:
            newGeometry.setTop(originalGeometry.top() + delta.y());
            break;
        case WindowBoundary::BOTTOM:
            newGeometry.setBottom(originalGeometry.bottom() + delta.y());
            break;
        case WindowBoundary::LEFT:
            newGeometry.setLeft(originalGeometry.left() + delta.x());
            break;
        case WindowBoundary::RIGHT:
            newGeometry.setRight(originalGeometry.right() + delta.x());
            break;
        case WindowBoundary::TOP_LEFT:
            newGeometry.setTop(originalGeometry.top() + delta.y());
            newGeometry.setLeft(originalGeometry.left() + delta.x());
            break;
        case WindowBoundary::TOP_RIGHT:
            newGeometry.setTop(originalGeometry.top() + delta.y());
            newGeometry.setRight(originalGeometry.right() + delta.x());
            break;
        case WindowBoundary::BOTTOM_LEFT:
            newGeometry.setBottom(originalGeometry.bottom() + delta.y());
            newGeometry.setLeft(originalGeometry.left() + delta.x());
            break;
        case WindowBoundary::BOTTOM_RIGHT:
            newGeometry.setBottom(originalGeometry.bottom() + delta.y());
            newGeometry.setRight(originalGeometry.right() + delta.x());
            break;
        }

        this->setGeometry(newGeometry);
    }
    QWidget::mouseMoveEvent(event);
}

void BrowserWindow::mouseReleaseEvent(QMouseEvent *event){
    this->resizing = false;
    QWidget::mouseReleaseEvent(event);
}

bool BrowserWindow::isEdgePosition(QPointF position){
    int offset = 3;
    if(position.x() <= this->width() && position.y() >= 0 && position.y() <= offset){
        return true;
    }else if(position.y() <= this->height() && position.x() >= 0 && position.x() <= offset){
        return true;
    }else if(position.x() <= this->width() && position.y() <= this->height() && position.y() >= this->height() - offset){
        return true;
    }else if(position.y() <= this->height() && position.x() <= this->width() && position.x() >= this->width() - offset){
        return true;
    }
    return false;
}

WindowBoundary BrowserWindow::edgePosition(QPointF position){
    int offset = 3;

    if((position.x() >= 0 && position.x() <= offset) && (position.y() >= 0 && position.y() <= offset)){
        return WindowBoundary::TOP_LEFT;
    }else if((position.x() >= this->width() - offset && position.x() <= this->width()) && (position.y() >= 0 && position.y() <= offset)){
        return WindowBoundary::TOP_RIGHT;
    }else if((position.x() >= 0 && position.x() <= offset) && (position.y() >= this->height() - offset && position.y() <= this->height())){
        return WindowBoundary::BOTTOM_LEFT;
    }else if((position.x() >= this->width() - offset && position.x() <= this->width()) && (position.y() >= this->height() - offset && position.y() <= this->height())){
        return WindowBoundary::BOTTOM_RIGHT;
    }

    if(position.x() <= this->width() && position.y() >= 0 && position.y() <= offset){
        return WindowBoundary::TOP;
    }else if(position.y() <= this->height() && position.x() >= 0 && position.x() <= offset){
        return WindowBoundary::LEFT;
    }else if(position.x() <= this->width() && position.y() <= this->height() && position.y() >= this->height() - offset){
        return WindowBoundary::BOTTOM;
    }else if(position.y() <= this->height() && position.x() <= this->width() && position.x() >= this->width() - offset){
        return WindowBoundary::RIGHT;
    }
    return WindowBoundary::NOT_BOUNDARY;
}

BrowserWindow::~BrowserWindow(){}
