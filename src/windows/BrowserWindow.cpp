#ifdef _WIN32
#include <windows.h>
#include <windowsx.h>
#endif

#include <QPainter>
#include <QWindow>

#include "BrowserWindow.h"
#include "../components/WebView.h"

#define EDGE_MARGIN 5

BrowserWindow::BrowserWindow(QSize size, QWidget *parent) : QMainWindow(parent), isMaximized(false) {
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMinimumSize(size);
    this->setMouseTracking(true);
    this->setContentsMargins(0,0,0,0);

    this->centralWidget = new QWidget();
    this->centralWidget->setContentsMargins(0, 0, 0, 0);
    this->centralWidget->setMouseTracking(true);
    this->layout = new QVBoxLayout(this->centralWidget);
    this->layout->setContentsMargins(5,0,5,5);
    this->layout->setSpacing(0);

    this->titleBar = new WindowTitleBar();

    WebView *test = new WebView(QWebEngineProfile::defaultProfile());
    test->load(QUrl("https://youtube.com"));

    this->layout->addWidget(this->titleBar);
    this->layout->addWidget(test);

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
    if (event->button() == Qt::LeftButton && this->isEdgePosition(event->position())) {
        this->windowHandle()->startSystemResize(this->getEdgePosition(event->position()));
    } else if (event->button() == Qt::LeftButton && event->position().y() <= 30) {
        this->windowHandle()->startSystemMove();
    }
    QMainWindow::mousePressEvent(event);
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

bool BrowserWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
#ifdef _WIN32
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG*>(message);
        switch (msg->message) {
        case WM_NCHITTEST:
            *result = 0;
            if (isEdgePosition(QPointF(GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam)))) {
                *result = HTCLIENT;
                return QWidget::nativeEvent(eventType, message, result);
            }
            break;
        case WM_GETMINMAXINFO:
            handleAeroSnap(msg, (long*) result);
            return true;
        default:
            break;
        }
    }
#endif
    return QWidget::nativeEvent(eventType, message, result);
}

#ifdef _WIN32
void BrowserWindow::handleAeroSnap(MSG *msg, long *result) {
    LPMINMAXINFO mmi = (LPMINMAXINFO)msg->lParam;

    // Get the work area of the monitor
    HMONITOR hMonitor = MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = { sizeof(MONITORINFO) };
    GetMonitorInfo(hMonitor, &mi);

    RECT rcWork = mi.rcWork;
    RECT rcMonitor = mi.rcMonitor;

    // Adjust the maximized size and position to account for the taskbar
    mmi->ptMaxPosition.x = rcWork.left - rcMonitor.left;
    mmi->ptMaxPosition.y = rcWork.top - rcMonitor.top;
    mmi->ptMaxSize.x = rcWork.right - rcWork.left;
    mmi->ptMaxSize.y = rcWork.bottom - rcWork.top;

    *result = 0;
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
}



