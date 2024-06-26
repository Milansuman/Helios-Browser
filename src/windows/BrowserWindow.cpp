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

#include "BrowserWindow.h"
#include "../components/TabManager.h"

#define EDGE_MARGIN 5

BrowserWindow::BrowserWindow(QSize size, QWidget *parent) : QMainWindow(parent), isMaximized(false) {
    #ifdef _WIN32
    this->nativeWindow = new WinNativeWindow(1  * window()->devicePixelRatio()
        , 1 * window()->devicePixelRatio()
        , 1 * window()->devicePixelRatio()
        , 1 * window()->devicePixelRatio());

    this->nativeWindowHandle = this->nativeWindow->hWnd;

    if(this->nativeWindowHandle){
        this->setProperty("_q_embedded_native_parent_handle", (WId)this->nativeWindowHandle);
        SetWindowLong((HWND)this->winId(), GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

        SetParent((HWND)this->winId(), this->nativeWindowHandle);
        QEvent e(QEvent::EmbeddingControl);
        QApplication::sendEvent(this, &e);
    }

    this->nativeWindow->childWindow = (HWND)this->winId();
    this->nativeWindow->childWidget = this;
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

    this->contentLayout = new QHBoxLayout();
    this->contentLayout->setContentsMargins(0,0,0,0);

    this->titleBar = new WindowTitleBar();
    this->tabManager = new TabManager();

    connect(this->titleBar, &WindowTitleBar::showGroupBar, this, [=](){
        this->tabManager->windowShowGroups();
    });

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

    this->sideBar = new SideBar();

    this->sideBarAnimation = new QPropertyAnimation(this->sideBar, "maximumWidth");
    this->sideBarAnimation->setDuration(150);
    this->sideBarAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    connect(this->titleBar, &WindowTitleBar::toggleSideBarRequested, this, &BrowserWindow::toggleSideBar);

    connect(this->sideBar, &SideBar::newGroupRequested, this, [=](){
        this->tabManager->addGroup();
    });

    this->contentLayout->addWidget(this->sideBar);
    this->contentLayout->addWidget(this->tabManager);

    this->layout->addWidget(this->titleBar);
    this->layout->addLayout(this->contentLayout);

    // Handle titlebar buttons
    connect(this->titleBar->minimizeButton(), &QPushButton::clicked, this, [=](){
        #ifdef __WIN32
        SendMessage(this->nativeWindowHandle, WM_SYSCOMMAND, SC_MINIMIZE, 0)
        #endif
        this->showMinimized();
    });
    connect(this->titleBar->maximizeButton(), &QPushButton::clicked, this, [=]() {
        if (this->isMaximized) {
            #ifdef __WIN32
            SendMessage(this->nativeWindowHandle, WM_SYSCOMMAND, SC_RESTORE, 0);
            #endif
            this->showNormal();
        } else {
            #ifdef __WIN32
            SendMessage(this->nativeWindowHandle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
            #endif
            this->showMaximized();
        }
        this->isMaximized = !this->isMaximized;
        this->update();  // Trigger a repaint when maximizing/unmaximizing
    });
    connect(this->titleBar->closeButton(), &QPushButton::clicked, this, &BrowserWindow::close);

    this->setCentralWidget(this->centralWidget);

    #ifdef __WIN32
    SendMessage(this->nativeWindowHandle, WM_SIZE, 0, 0);
    #endif
}

void BrowserWindow::toggleSideBar() {
    if (this->sideBar->isVisible()) {
        this->sideBarAnimation->setStartValue(this->sideBar->width());
        this->sideBarAnimation->setEndValue(0);
        connect(this->sideBarAnimation, &QPropertyAnimation::finished, this, &BrowserWindow::hideSideBar);
        this->sideBarAnimation->start();
    } else {
        this->sideBar->show();
        this->sideBarAnimation->setStartValue(0);
        this->sideBarAnimation->setEndValue(200);
        disconnect(this->sideBarAnimation, &QPropertyAnimation::finished, this, &BrowserWindow::hideSideBar);
        this->sideBarAnimation->start();
    }
}

void BrowserWindow::hideSideBar() {
    this->sideBar->hide();
}

#ifdef __linux__
void BrowserWindow::show(){
    QMainWindow::show();

    QPainterPath *path = new QPainterPath();
    path->addRoundedRect(rect(), 10, 10);

    if (QWindow *window = windowHandle()) {
        KWindowEffects::enableBlurBehind(window, true, QRegion(path->toFillPolygon().toPolygon()));
    }else{
        qDebug() << this->windowHandle();
    }
}

void BrowserWindow::resizeEvent(QResizeEvent *event){
    QPainterPath *path = new QPainterPath();
    path->addRoundedRect(rect(), 10, 10);

    if (QWindow *window = windowHandle()) {
        KWindowEffects::enableBlurBehind(window, true, QRegion(path->toFillPolygon().toPolygon()));
    }else{
        qDebug() << this->windowHandle();
    }
}
#endif

void BrowserWindow::paintEvent(QPaintEvent *event) {
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Setting color and rounded corners
    painter.setBrush(QBrush(QColor(30, 30, 30, 148)));
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



#ifdef __WIN32
bool BrowserWindow::nativeEvent(const QByteArray &, void *message, long *result){
    MSG *msg = (MSG *)message;

    if (msg->message == WM_SETFOCUS)
    {
        Qt::FocusReason reason;
        if (::GetKeyState(VK_LBUTTON) < 0 || ::GetKeyState(VK_RBUTTON) < 0)
            reason = Qt::MouseFocusReason;
        else if (::GetKeyState(VK_SHIFT) < 0)
            reason = Qt::BacktabFocusReason;
        else
            reason = Qt::TabFocusReason;
        QFocusEvent e(QEvent::FocusIn, reason);
        QApplication::sendEvent(this, &e);
    }

    //Only close if safeToClose clears()
    if (msg->message == WM_CLOSE)
    {
		if (true /* put your check for it if it safe to close your app here */) //eg, does the user need to save a document
		{
			//Safe to close, so hide the parent window
			ShowWindow(this->nativeWindow, false);

			//And then quit
			QApplication::quit();
		}
		else
        {
            *result = 0; //Set the message to 0 to ignore it, and thus, don't actually close
            return true;
        }
    }

    //Double check WM_SIZE messages to see if the parent native window is maximized
    if (msg->message == WM_SIZE)
    {
        if (p_Widget && p_Widget->maximizeButton)
        {
			//Get the window state
            WINDOWPLACEMENT wp;
            GetWindowPlacement(this->nativeWindow, &wp);

			//If we're maximized, 
            if (wp.showCmd == SW_MAXIMIZE)
            {
                //Maximize button should show as Restore
                p_Widget->maximizeButton->setChecked(true);
            }
            else
            {
                //Maximize button should show as Maximize
                p_Widget->maximizeButton->setChecked(false);
            }
        }
    }

    //Pass NCHITTESTS on the window edges as determined by BORDERWIDTH & TOOLBARHEIGHT through to the parent native window
    if (msg->message == WM_NCHITTEST)
    {
        RECT WindowRect;
        int x, y;

        GetWindowRect(msg->hwnd, &WindowRect);
        x = GET_X_LPARAM(msg->lParam) - WindowRect.left;
        y = GET_Y_LPARAM(msg->lParam) - WindowRect.top;

        if (x >= BORDERWIDTH && x <= WindowRect.right - WindowRect.left - BORDERWIDTH && y >= BORDERWIDTH && y <= TOOLBARHEIGHT)
		{
            if (p_Widget->toolBar)
            {
                //If the mouse is over top of the toolbar area BUT is actually positioned over a child widget of the toolbar, 
				//Then we don't want to enable dragging. This allows for buttons in the toolbar, eg, a Maximize button, to keep the mouse interaction
				if (QApplication::widgetAt(QCursor::pos()) != p_Widget->toolBar)
                    return false;
	    	}
			
				//The mouse is over the toolbar area & is NOT over a child of the toolbar, so pass this message 
				//through to the native window for HTCAPTION dragging
				*result = HTTRANSPARENT;
				return true;

        }
        else if (x < BORDERWIDTH && y < BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x > WindowRect.right - WindowRect.left - BORDERWIDTH && y < BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x > WindowRect.right - WindowRect.left - BORDERWIDTH && y > WindowRect.bottom - WindowRect.top - BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x < BORDERWIDTH && y > WindowRect.bottom - WindowRect.top - BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x < BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (y < BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x > WindowRect.right - WindowRect.left - BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (y > WindowRect.bottom - WindowRect.top - BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }

        return false;
    }

    return false;
}

bool BrowserWindow::eventFilter(QObject *o, QEvent *e)
{
    QWidget *w = (QWidget*)o;

    switch (e->type())
    {
        case QEvent::WindowDeactivate:
        if (w->isModal() && w->isHidden())
            BringWindowToTop(this->nativeWindowHandle);
        break;

        case QEvent::Hide:
        if (_reenableParent) {
            EnableWindow(this->nativeWindowHandle, true);
            _reenableParent = false;
        }
        resetFocus();

        if (w->testAttribute(Qt::WA_DeleteOnClose) && w->isWindow())
            deleteLater();
        break;

        case QEvent::Show:
        if (w->isWindow()) {
            saveFocus();
            hide();
            if (w->isModal() && !_reenableParent) {
            EnableWindow(this->nativeWindowHandle, false);
            _reenableParent = true;
            }
        }
        break;

        case QEvent::Close:
        {
            ::SetActiveWindow(this->nativeWindowHandle);
            if (w->testAttribute(Qt::WA_DeleteOnClose))
                deleteLater();
            break;
        }
        default:
        break;
    }

    return QWidget::eventFilter(o, e);
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
    delete this->sideBar;
    delete this->contentLayout;
    delete this->layout;
    delete this->centralWidget;
    delete this->sideBarAnimation;
}



