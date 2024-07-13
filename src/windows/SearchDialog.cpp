#include "SearchDialog.h"
#include <QDialog>
#include <QHBoxLayout>
#include <QPixmap>
#include <QLineEdit>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QKeyEvent>
#include <QUrl>

#ifdef __linux__
#include <kwindoweffects.h>
#endif

#ifdef _WIN32
#include <dwmapi.h>
#include <windows.h>
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "user32.lib")

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

SearchDialog::SearchDialog(QWidget *parent) : QDialog(parent), searchText("")
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedWidth(500);
    this->setFocus();

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(30, 30, 30, 170));
    setPalette(palette);

    searchLayout = new QHBoxLayout();
    searchLayout->setContentsMargins(10, 10, 10, 10);

    searchIconLabel = new QLabel();
    QPixmap searchIcon(":/icons/white/search.png");
    searchIconLabel->setPixmap(searchIcon.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    searchIconLabel->setFixedSize(20, 20);

    // Search input
    searchbar = new QLineEdit();
    searchbar->setPlaceholderText("Search or Enter URL");
    searchbar->setStyleSheet(
        "QLineEdit{"
        "   background: transparent;"
        "   color: rgb(255, 255, 255);"
        "   border: none;"
        "}");
    this->setFocusProxy(this->searchbar);

    searchLayout->addWidget(searchIconLabel);
    searchLayout->addWidget(searchbar);
    this->setLayout(searchLayout);
}

void SearchDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw the background
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(rect(), 10, 10);
    painter.fillPath(backgroundPath, palette().window());

    // Draw the border
    QPainterPath borderPath;
    QRectF borderRect = rect();
    borderRect.adjust(0.5, 0.5, -0.5, -0.5);
    borderPath.addRoundedRect(borderRect, 10, 10);
    QPen borderPen(QColor(190, 190, 190));
    borderPen.setWidthF(1.0);
    painter.setPen(borderPen);
    painter.drawPath(borderPath);

    QDialog::paintEvent(event);
}

void SearchDialog::open()
{
    QDialog::open();
#ifdef __linux__
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    KWindowEffects::enableBlurBehind(this->windowHandle(), true, QRegion(path.toFillPolygon().toPolygon()));
#elif defined(_WIN32)
    enableBlurBehind();
#endif
}

void SearchDialog::setUrl(QUrl url)
{
    this->searchbar->setText(url.toString());
    this->searchText = url.toString();
}

void SearchDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        this->searchText = this->searchbar->text();
        this->accept();
        this->searchText = "";
        this->searchbar->setText("");
    }
}

QString SearchDialog::getSearch()
{
    return (QUrl::fromUserInput(this->searchText)).toString();
}

SearchDialog::~SearchDialog()
{
    delete this->searchIconLabel;
    delete this->searchLayout;
    delete this->searchbar;
}

#ifdef _WIN32
void SearchDialog::enableBlurBehind()
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