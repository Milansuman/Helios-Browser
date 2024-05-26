#include "browserwindow.h"
#include "components/titlebarbuttons.h"
#include "components/addressbox.h"
#include <QMainWindow>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QWindow>

#define EDGE_MARGIN 5

BrowserWindow::BrowserWindow(QWidget *parent, double width, double height): QMainWindow(parent){
    this->resize(width, height);
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    //Implement Outer UI
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setMouseTracking(true);

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
        this->windowHandle()->startSystemResize(this->getEdgePosition(event->globalPosition()));
    }else if(event->button() == Qt::LeftButton && event->position().y() <= 20){
        this->windowHandle()->startSystemMove();
    }
    QWidget::mousePressEvent(event);
}

void BrowserWindow::mouseMoveEvent(QMouseEvent *event){
    switch(this->getEdgePosition(event->globalPosition())){
        case Qt::TopEdge:
        case Qt::BottomEdge:
            this->setCursor(Qt::SizeVerCursor);
            break;
        case Qt::RightEdge:
        case Qt::LeftEdge:
            this->setCursor(Qt::SizeHorCursor);
            break;
        case Qt::TopEdge | Qt::LeftEdge:
        case Qt::BottomEdge | Qt::RightEdge:
            this->setCursor(Qt::SizeFDiagCursor);
            break;
        case Qt::TopEdge | Qt::RightEdge:
        case Qt::BottomEdge | Qt::LeftEdge:
            this->setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            this->setCursor(Qt::ArrowCursor);
    }
    QMainWindow::mouseMoveEvent(event);
}

void BrowserWindow::mouseReleaseEvent(QMouseEvent *event){
    this->resizing = false;
    this->moving = false;
    QWidget::mouseReleaseEvent(event);
}

bool BrowserWindow::isEdgePosition(QPointF position){
    if(position.x() <= this->width() && position.y() >= 0 && position.y() <= EDGE_MARGIN){
        return true;
    }else if(position.y() <= this->height() && position.x() >= 0 && position.x() <= EDGE_MARGIN){
        return true;
    }else if(position.x() <= this->width() && position.y() <= this->height() && position.y() >= this->height() - EDGE_MARGIN){
        return true;
    }else if(position.y() <= this->height() && position.x() <= this->width() && position.x() >= this->width() - EDGE_MARGIN){
        return true;
    }
    return false;
}

QFlags<Qt::Edge> BrowserWindow::getEdgePosition(QPointF position){
    if((position.x() >= 0 && position.x() <= EDGE_MARGIN) && (position.y() >= 0 && position.y() <= EDGE_MARGIN)){
        return Qt::TopEdge | Qt::LeftEdge;
    }else if((position.x() >= this->width() - EDGE_MARGIN && position.x() <= this->width()) && (position.y() >= 0 && position.y() <= EDGE_MARGIN)){
        return Qt::TopEdge | Qt::RightEdge;
    }else if((position.x() >= 0 && position.x() <= EDGE_MARGIN) && (position.y() >= this->height() - EDGE_MARGIN && position.y() <= this->height())){
        return Qt::BottomEdge | Qt::LeftEdge;
    }else if((position.x() >= this->width() - EDGE_MARGIN && position.x() <= this->width()) && (position.y() >= this->height() - EDGE_MARGIN && position.y() <= this->height())){
        return Qt::BottomEdge | Qt::RightEdge;
    }

    if(position.x() <= this->width() && position.y() >= 0 && position.y() <= EDGE_MARGIN){
        return Qt::TopEdge;
    }else if(position.y() <= this->height() && position.x() >= 0 && position.x() <= EDGE_MARGIN){
        return Qt::LeftEdge;
    }else if(position.x() <= this->width() && position.y() <= this->height() && position.y() >= this->height() - EDGE_MARGIN){
        return Qt::BottomEdge;
    }else if(position.y() <= this->height() && position.x() <= this->width() && position.x() >= this->width() - EDGE_MARGIN){
        return Qt::RightEdge;
    }

    return QFlags<Qt::Edge>();
}

BrowserWindow::~BrowserWindow(){}