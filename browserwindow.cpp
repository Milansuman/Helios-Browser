#include "browserwindow.h"
#include "components/titlebar.h"
#include "components/addressbox.h"
#include "components/webview.h"
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

    this->search = new SearchDialog(this);

    //Implement Outer UI
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setMouseTracking(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(5,0,5,5);
    
    TitleBar *titlebar = new TitleBar(this);

    //=======================CONTENT==========================================
    QHBoxLayout *contentLayout = new QHBoxLayout;
    WebView *webview = new WebView();
    webview->load(QUrl("https://youtube.com"));
    webview->setMouseTracking(true);

    QObject::connect(this->search, &SearchDialog::accepted, this, [=](){
        webview->load(QUrl(this->search->getSearch()));
    });

    // WebView *devTools = new WebView();
    // devTools->page()->setInspectedPage(webview->page());

    contentLayout->addWidget(webview);
    // contentLayout->addWidget(devTools);
    mainLayout->addWidget(titlebar);
    mainLayout->addLayout(contentLayout);

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

void BrowserWindow::mouseReleaseEvent(QMouseEvent *event){
    QWidget::mouseReleaseEvent(event);
}

void BrowserWindow::resizeEvent(QResizeEvent *event){
    
}

void BrowserWindow::showSearchDialog(){
    this->search->open();
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