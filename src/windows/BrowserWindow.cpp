#include <QPainter>
#include <QWindow>
#include <QPainterPath>
#include <QRegion>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QApplication>

#ifdef __linux__
#include <kwindoweffects.h>
#endif

#ifdef _WIN32
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "user32.lib")

// Windows-specific structures and functions
struct ACCENTPOLICY
{
    int nAccentState;
    int nFlags;
    int nColor;
    int nAnimationId;
};
struct WINCOMPATTRDATA
{
    int nAttribute;
    PVOID pData;
    ULONG ulDataSize;
};

typedef BOOL(WINAPI *pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA *);
#endif

#include "BrowserWindow.h"
#include "../components/TabManager.h"

#define EDGE_MARGIN 5

BrowserWindow::BrowserWindow(QUrl url, QSize size, QWidget *parent) : BrowserWindow(size, parent)
{
    this->tabManager->setInitialUrl(url);
}

BrowserWindow::BrowserWindow(QSize size, QWidget *parent) : QMainWindow(parent), isMaximized(false)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(size);
    this->setMouseTracking(true);
    this->setContentsMargins(0, 0, 0, 0);

    this->centralWidget = new QWidget();
    this->centralWidget->setContentsMargins(0, 0, 0, 0);
    this->centralWidget->setMouseTracking(true);

    this->layout = new QVBoxLayout(this->centralWidget);
    this->layout->setContentsMargins(5, 0, 5, 5);
    this->layout->setSpacing(0);

    this->contentLayout = new QHBoxLayout();
    this->contentLayout->setContentsMargins(0, 0, 0, 0);

    this->titleBar = new WindowTitleBar();
    this->titleBar->setMouseTracking(true);
    this->tabManager = new TabManager();

    connect(this->titleBar, &WindowTitleBar::showGroupBar, this, [=]()
            { this->tabManager->windowShowGroups(); });

    connect(this->titleBar, &WindowTitleBar::downloadRequested, this, [=]()
            { this->tabManager->DownloadShowMenu(); });

    connect(this->titleBar, &WindowTitleBar::splitTabLeftRequested, this, [=]()
            { this->tabManager->windowSplitLeft(); });

    connect(this->titleBar, &WindowTitleBar::splitTabRightRequested, this, [=]()
            { this->tabManager->windowSplitRight(); });

    connect(this->titleBar, &WindowTitleBar::searchRequested, this, [=]()
            { this->tabManager->windowSearch(); });

    connect(this->titleBar, &WindowTitleBar::nextPageRequested, this, [=]()
            { this->tabManager->windowNextPage(); });

    connect(this->titleBar, &WindowTitleBar::previousPageRequested, this, [=]()
            { this->tabManager->windowPreviousPage(); });

    connect(this->titleBar, &WindowTitleBar::reloadRequested, this, [=]()
            { this->tabManager->windowReload(); });

    connect(this->titleBar, &WindowTitleBar::copyLinkRequested, this, [=]()
            { this->tabManager->windowCopyLink(); });

    connect(this->titleBar, &WindowTitleBar::showSiteSettingsRequested, this, [=]()
            { this->tabManager->windowShowSiteSettings(); });

    connect(this->tabManager, &TabManager::titleChanged, this, [=](QString title)
            { this->titleBar->setTitle(title); });

    connect(this->tabManager, &TabManager::displayTitleBarOnWindowRequested, this, [=]()
            { this->titleBar->setTitleBarVisible(true); });

    connect(this->tabManager, &TabManager::hideTitleBarOnWindowRequested, this, [=]()
            { this->titleBar->setTitleBarVisible(false); });

    connect(this->tabManager, &TabManager::newWindowRequested, this, [=](QUrl url)
            {
        BrowserWindow *newWindow = new BrowserWindow(url, QSize(1000, 900));
        newWindow->show(); });

    this->sideBar = new SideBar();
    this->sideBar->setMouseTracking(true);

    this->sideBarAnimation = new QPropertyAnimation(this->sideBar, "maximumWidth");
    this->sideBarAnimation->setDuration(150);
    this->sideBarAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    connect(this->titleBar, &WindowTitleBar::toggleSideBarRequested, this, &BrowserWindow::toggleSideBar);

    connect(this->sideBar, &SideBar::newGroupRequested, this, [=]()
            { this->tabManager->addGroup(); });

    this->contentLayout->addWidget(this->sideBar);
    this->contentLayout->addWidget(this->tabManager);

    this->layout->addWidget(this->titleBar);
    this->layout->addLayout(this->contentLayout);

    // Handle titlebar buttons
    connect(this->titleBar->minimizeButton(), &QPushButton::clicked, this, [=]()
            { this->showMinimized(); });
    connect(this->titleBar->maximizeButton(), &QPushButton::clicked, this, [=]()
            {
                if (this->isMaximized)
                {
                    this->showNormal();
                }
                else
                {
                    this->showMaximized();
                }
                this->isMaximized = !this->isMaximized;
                this->update(); // Trigger a repaint when maximizing/unmaximizing
            });
    connect(this->titleBar->closeButton(), &QPushButton::clicked, this, &BrowserWindow::close);

    this->setCentralWidget(this->centralWidget);
}

void BrowserWindow::toggleSideBar()
{
    if (this->sideBar->isVisible())
    {
        this->sideBarAnimation->setStartValue(this->sideBar->width());
        this->sideBarAnimation->setEndValue(0);
        connect(this->sideBarAnimation, &QPropertyAnimation::finished, this, &BrowserWindow::hideSideBar);
        this->sideBarAnimation->start();
    }
    else
    {
        this->sideBar->show();
        this->sideBarAnimation->setStartValue(0);
        this->sideBarAnimation->setEndValue(200);
        disconnect(this->sideBarAnimation, &QPropertyAnimation::finished, this, &BrowserWindow::hideSideBar);
        this->sideBarAnimation->start();
    }
}

void BrowserWindow::hideSideBar()
{
    this->sideBar->hide();
}
void BrowserWindow::show()
{
    QMainWindow::show();

#ifdef __linux__
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);

    if (QWindow *window = windowHandle())
    {
        KWindowEffects::enableBlurBehind(window, true, QRegion(path.toFillPolygon().toPolygon()));
    }
    else
    {
        qDebug() << this->windowHandle();
    }
#elif defined(_WIN32)
    enableBlurBehind();
#endif
}

#ifdef _WIN32
void BrowserWindow::enableBlurBehind()
{
    HWND hwnd = (HWND)this->winId();

    HMODULE hUser = GetModuleHandle(L"user32.dll");
    if (hUser)
    {
        pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = {3, 0, 0, 0}; // ACCENT_ENABLE_BLURBEHIND
            WINCOMPATTRDATA data = {19, &policy, sizeof(ACCENTPOLICY)};
            SetWindowCompositionAttribute(hwnd, &data);
        }
    }
}
#endif

void BrowserWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

#ifdef __linux__
    QPainterPath *path = new QPainterPath();
    path->addRoundedRect(rect(), 10, 10);

    if (QWindow *window = windowHandle())
    {
        KWindowEffects::enableBlurBehind(window, true, QRegion(path->toFillPolygon().toPolygon()));
    }
    else
    {
        qDebug() << this->windowHandle();
    }
#elif defined(_WIN32)
    enableBlurBehind();
#endif
}

void BrowserWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Setting color and rounded corners
    painter.setBrush(QBrush(QColor(30, 30, 30, 148)));
    painter.setPen(Qt::NoPen);
    QRect roundedRect = rect();
    if (!this->isMaximized)
    {
        roundedRect.adjust(1, 1, -1, -1);
        painter.drawRoundedRect(roundedRect, 10, 10);
    }
    else
    {
        painter.drawRect(rect());
    }
}

void BrowserWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (this->isEdgePosition(event->position()))
        {
            this->windowHandle()->startSystemResize(this->getEdgePosition(event->position()));
        }
        else if (event->position().y() <= 30)
        {
            this->windowHandle()->startSystemMove();
        }
    }
    QMainWindow::mousePressEvent(event);
}

void BrowserWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->position().y() <= 30)
    {
        if (this->isMaximized)
        {
            this->showNormal();
        }
        else
        {
            this->showMaximized();
        }
        this->isMaximized = !this->isMaximized;
    }
    QMainWindow::mouseDoubleClickEvent(event);
}

void BrowserWindow::mouseMoveEvent(QMouseEvent *event)
{
    switch (this->getEdgePosition(event->position()))
    {
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

bool BrowserWindow::isEdgePosition(QPointF position)
{
    if (position.x() >= 0 && position.x() <= EDGE_MARGIN)
    {
        return true;
    }
    else if (position.x() >= this->width() - EDGE_MARGIN && position.x() <= this->width())
    {
        return true;
    }
    else if (position.y() >= 0 && position.y() <= EDGE_MARGIN)
    {
        return true;
    }
    else if (position.y() >= this->height() - EDGE_MARGIN && position.y() <= this->height())
    {
        return true;
    }
    return false;
}

QFlags<Qt::Edge> BrowserWindow::getEdgePosition(QPointF position)
{
    if ((position.x() >= 0 && position.x() <= EDGE_MARGIN) && (position.y() >= 0 && position.y() <= EDGE_MARGIN))
    {
        return Qt::TopEdge | Qt::LeftEdge;
    }
    else if ((position.x() >= this->width() - EDGE_MARGIN && position.x() <= this->width()) && (position.y() >= 0 && position.y() <= EDGE_MARGIN))
    {
        return Qt::TopEdge | Qt::RightEdge;
    }
    else if ((position.x() >= 0 && position.x() <= EDGE_MARGIN) && (position.y() >= this->height() - EDGE_MARGIN && position.y() <= this->height()))
    {
        return Qt::BottomEdge | Qt::LeftEdge;
    }
    else if ((position.x() >= this->width() - EDGE_MARGIN && position.x() <= this->width()) && (position.y() >= this->height() - EDGE_MARGIN && position.y() <= this->height()))
    {
        return Qt::BottomEdge | Qt::RightEdge;
    }
    else if (position.x() >= 0 && position.x() <= EDGE_MARGIN)
    {
        return Qt::LeftEdge;
    }
    else if (position.x() >= this->width() - EDGE_MARGIN && position.x() <= this->width())
    {
        return Qt::RightEdge;
    }
    else if (position.y() >= 0 && position.y() <= EDGE_MARGIN)
    {
        return Qt::TopEdge;
    }
    else if (position.y() >= this->height() - EDGE_MARGIN && position.y() <= this->height())
    {
        return Qt::BottomEdge;
    }

    return QFlags<Qt::Edge>();
}

BrowserWindow::~BrowserWindow()
{
    delete this->titleBar;
    delete this->tabManager;
    delete this->sideBar;
    delete this->contentLayout;
    delete this->layout;
    delete this->centralWidget;
    delete this->sideBarAnimation;
}
