#ifdef _WIN32
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#define FIXED_WIDTH(widget) (widget->minimumWidth() >= widget->maximumWidth())
#define FIXED_HEIGHT(widget) (widget->minimumHeight() >= widget->maximumHeight())
#define FIXED_SIZE(widget) (FIXED_WIDTH(widget) && FIXED_HEIGHT(widget))
#endif

#include <QPainter>
#include <QWindow>

#include "BrowserWindow.h"
#include "../components/TabManager.h"

#define EDGE_MARGIN 5

BrowserWindow::BrowserWindow(QSize size, QWidget *parent) : QMainWindow(parent), isMaximized(false) {
    #ifdef _WIN32
    HINSTANCE hInstance = GetModuleHandleW(nullptr);

    HWND parent_hwnd = nullptr;

    if (parent)
        parent_hwnd = HWND(parent->winId());
    WNDCLASSEXW wcx;
    memset(&wcx, 0, sizeof(WNDCLASSEXW));
    wcx.cbSize = sizeof(WNDCLASSEXW);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.hInstance = hInstance;
    wcx.lpfnWndProc = WNDPROC(WndProc);
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hCursor	= LoadCursorW(nullptr, IDC_ARROW);

    wcx.lpszClassName = L"BrowserWindow";

    RegisterClassExW(&wcx);

    this->windowID = CreateWindowW(wcx.lpszClassName, nullptr,
                           WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                           parent_hwnd, nullptr, hInstance, nullptr);
    if (!this->windowID)
    {
        qDebug()<<"error occured runnnnn";
    }

    SetWindowLongPtrW(this->windowID, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    #endif

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(size);
    this->setMouseTracking(true);
    this->setContentsMargins(0,0,0,0);

    this->centralWidget = new QWidget();
    this->centralWidget->setContentsMargins(0, 0, 0, 0);
    this->centralWidget->setMouseTracking(true);
    this->layout = new QVBoxLayout(this->centralWidget);
    this->layout->setContentsMargins(5,0,5,5);
    this->layout->setSpacing(0);

    this->titleBar = new WindowTitleBar();
    this->tabManager = new TabManager();

    connect(this->titleBar, &WindowTitleBar::splitTabLeftRequested, this, [=](){
        this->tabManager->windowSplitLeft();
    });

    connect(this->titleBar, &WindowTitleBar::splitTabRightRequested, this, [=](){
        this->tabManager->windowSplitRight();
    });

    connect(this->titleBar, &WindowTitleBar::searchRequested, this, [=](){
        this->tabManager->windowSearch();
    });

    connect(this->titleBar, &WindowTitleBar::nextPageRequested, this, [=](){
        this->tabManager->windowNextPage();
    });

    connect(this->titleBar, &WindowTitleBar::previousPageRequested, this, [=](){
        this->tabManager->windowPreviousPage();
    });

    connect(this->titleBar, &WindowTitleBar::reloadRequested, this, [=](){
        this->tabManager->windowReload();
    });

    connect(this->titleBar, &WindowTitleBar::copyLinkRequested, this, [=](){
        this->tabManager->windowCopyLink();
    });

    connect(this->tabManager, &TabManager::titleChanged, this, [=](QString title){
        this->titleBar->setTitle(title);
    });

    connect(this->tabManager, &TabManager::displayTitleBarOnWindowRequested, this, [=](){
        this->titleBar->setTitleBarVisible(true);
    });

    connect(this->tabManager, &TabManager::hideTitleBarOnWindowRequested, this, [=](){
        this->titleBar->setTitleBarVisible(false);
    });

    this->layout->addWidget(this->titleBar);
    this->layout->addWidget(this->tabManager);

    // Handle titlebar buttons
    connect(this->titleBar->minimizeButton(), &QPushButton::clicked, this, &BrowserWindow::showMinimized);
    connect(this->titleBar->maximizeButton(), &QPushButton::clicked, this, [=]() {
        if (this->isMaximized) {
            this->showNormal();
        } else {
            this->showMaximized();
        }
        this->isMaximized = !this->isMaximized;
        this->update();  // Trigger a repaint when maximizing/unmaximizing
    });
    connect(this->titleBar->closeButton(), &QPushButton::clicked, this, &BrowserWindow::close);

    this->setCentralWidget(this->centralWidget);
}

void BrowserWindow::paintEvent(QPaintEvent *event) {
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Setting color and rounded corners
    painter.setBrush(QBrush(QColor(30, 30, 30)));
    painter.setPen(Qt::NoPen);
    QRect roundedRect = rect();
    if (!this->isMaximized) {
        roundedRect.adjust(1, 1, -1, -1);
        painter.drawRoundedRect(roundedRect, 10, 10);
    } else {
        painter.drawRect(rect());
    }
}

void BrowserWindow::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton){
        if (this->isEdgePosition(event->position())) {
            this->windowHandle()->startSystemResize(this->getEdgePosition(event->position()));
        } else if (event->position().y() <= 30) {
            this->windowHandle()->startSystemMove();
        }
    }
    QMainWindow::mousePressEvent(event);
}

void BrowserWindow::mouseDoubleClickEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && event->position().y() <= 30){
        if (this->isMaximized) {
            this->showNormal();
        } else {
            this->showMaximized();
        }
        this->isMaximized = !this->isMaximized;
    }
    QMainWindow::mouseDoubleClickEvent(event);
}

void BrowserWindow::mouseMoveEvent(QMouseEvent *event) {
    switch (this->getEdgePosition(event->position())) {
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



#ifdef _WIN32
LRESULT CALLBACK BrowserWindow::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    BrowserWindow *window = reinterpret_cast<BrowserWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if(!window){
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}
#endif

bool BrowserWindow::isEdgePosition(QPointF position) {
    if (position.x() >= 0 && position.x() <= EDGE_MARGIN) {
        return true;
    } else if (position.x() >= this->width() - EDGE_MARGIN && position.x() <= this->width()) {
        return true;
    } else if (position.y() >= 0 && position.y() <= EDGE_MARGIN) {
        return true;
    } else if (position.y() >= this->height() - EDGE_MARGIN && position.y() <= this->height()) {
        return true;
    }
    return false;
}

QFlags<Qt::Edge> BrowserWindow::getEdgePosition(QPointF position) {
    if ((position.x() >= 0 && position.x() <= EDGE_MARGIN) && (position.y() >= 0 && position.y() <= EDGE_MARGIN)) {
        return Qt::TopEdge | Qt::LeftEdge;
    } else if ((position.x() >= this->width() - EDGE_MARGIN && position.x() <= this->width()) && (position.y() >= 0 && position.y() <= EDGE_MARGIN)) {
        return Qt::TopEdge | Qt::RightEdge;
    } else if ((position.x() >= 0 && position.x() <= EDGE_MARGIN) && (position.y() >= this->height() - EDGE_MARGIN && position.y() <= this->height())) {
        return Qt::BottomEdge | Qt::LeftEdge;
    } else if ((position.x() >= this->width() - EDGE_MARGIN && position.x() <= this->width()) && (position.y() >= this->height() - EDGE_MARGIN && position.y() <= this->height())) {
        return Qt::BottomEdge | Qt::RightEdge;
    } else if (position.x() >= 0 && position.x() <= EDGE_MARGIN) {
        return Qt::LeftEdge;
    } else if (position.x() >= this->width() - EDGE_MARGIN && position.x() <= this->width()) {
        return Qt::RightEdge;
    } else if (position.y() >= 0 && position.y() <= EDGE_MARGIN) {
        return Qt::TopEdge;
    } else if (position.y() >= this->height() - EDGE_MARGIN && position.y() <= this->height()) {
        return Qt::BottomEdge;
    }

    return QFlags<Qt::Edge>();
}

BrowserWindow::~BrowserWindow() {
    delete this->titleBar;
    delete this->tabManager;
    delete this->layout;
    delete this->centralWidget;
}



