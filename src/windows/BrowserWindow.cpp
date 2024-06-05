#ifdef _WIN32
#include <windows.h>
#endif

#include <QPainter>
#include <QWindow>

#include "BrowserWindow.h"

#define EDGE_MARGIN 5

BrowserWindow::BrowserWindow(QSize size, QWidget *parent): QMainWindow(parent), isMaximized(false){
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMinimumSize(size);
    this->setMouseTracking(true);

    this->centralWidget = new QWidget();
    this->centralWidget->setMouseTracking(true);
    this->layout = new QVBoxLayout(this->centralWidget);

    this->titleBar = new WindowTitleBar();

    this->layout->addWidget(this->titleBar);
    this->layout->addStretch();

    //handle titlebar buttons
    this->connect(this->titleBar->minimizeButton(), &QPushButton::clicked, this, [=](){
        this->showMinimized();
    });

    this->connect(this->titleBar->maximizeButton(), &QPushButton::clicked, this, [=](){
        if(this->isMaximized){
            this->showNormal();
        }else{
            this->showMaximized();
        }
        this->isMaximized = !this->isMaximized;
    });

    this->connect(this->titleBar->closeButton(), &QPushButton::clicked, this, [=](){
        this->close();
    });

    this->setCentralWidget(this->centralWidget);
}

void BrowserWindow::paintEvent(QPaintEvent *event){
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //setting color and rounded corners
    painter.setBrush(QBrush(QColor(30, 30, 30)));
    painter.setPen(Qt::NoPen);
    if(this->isMaximized){
        painter.drawRect(rect());
    }else{
        painter.drawRoundedRect(rect(), 10, 10);
    }
}

void BrowserWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && this->isEdgePosition(event->position())){
        this->windowHandle()->startSystemResize(this->getEdgePosition(event->position()));
    }else if(event->button() == Qt::LeftButton && event->position().y() <= 30){
        this->windowHandle()->startSystemMove();
    }
    QWidget::mousePressEvent(event);
}

void BrowserWindow::mouseMoveEvent(QMouseEvent *event){
    switch(this->getEdgePosition(event->position())){
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

bool BrowserWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result){
    #ifdef _WIN32

    if(eventType == "windows_generic_MSG"){
        MSG *msg = static_cast<MSG*>(message);
        switch (msg->message)
        {
        case WM_NCHITTEST:
            *result = 0;
            return true;
            break;
        case WM_GETMINMAXINFO;
            this->handleAeroSnap(msg, result);
            return true;
            break;
        default:
            break;
        }
    }

    #endif
    return QWidget::nativeEvent(eventType, message, result);
}

#ifdef _WIN32
void BrowserWindow::handleAeroSnap(MSG *msg, qintptr result){
    LPMINMAXINFO mmi = (LPMINMAXINFO)msg->lParam;

    // Get the work area of the monitor
    HMONITOR hMonitor = MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = { sizeof(MONITORINFO) };
    GetMonitorInfo(hMonitor, &mi);

    mmi->ptMaxPosition.x = mi.rcWork.left;
    mmi->ptMaxPosition.y = mi.rcWork.top;
    mmi->ptMaxSize.x = mi.rcWork.right - mi.rcWork.left;
    mmi->ptMaxSize.y = mi.rcWork.bottom - mi.rcWork.top;

    *result = 0;
}
#endif

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

BrowserWindow::~BrowserWindow(){
    delete this->titleBar;
}