#include "DownloadManager.h"

#include <QPainter>
#include <QPainterPath>
#include <QFontDatabase>
#include <QFont>

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

DownloadItem::DownloadItem(QWidget *parent): QWidget(parent){
    int titleFontId = QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    int subTextFontId = QFontDatabase::addApplicationFont(":/fonts/SFUIText-Regular.ttf");

    QFont titleFont(QFontDatabase::applicationFontFamilies(titleFontId).at(0), 8, QFont::Bold);
    QFont subTextFont(QFontDatabase::applicationFontFamilies(subTextFontId).at(0), 8, QFont::Normal);

    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(10, 10, 10, 10);
    this->subLayout = new QVBoxLayout();
    this->subLayout->setSpacing(2);

    this->fileName = new QLabel("VERY BIG FILE NAME AAAAAAAAAAAAA.png");
    this->fileName->setFont(titleFont);

    this->progressTextLayout = new QHBoxLayout();
    this->progressLabel = new QLabel("10MB/50GB");
    this->progressLabel->setFont(subTextFont);
    this->progressLabel->setStyleSheet(
        "QLabel{"
        "   color: rgb(164, 164, 164);"
        "}"
    );

    this->timeLabel = new QLabel("10d 10h 10m 10s");
    this->timeLabel->setFont(subTextFont);
    this->timeLabel->setStyleSheet(
        "QLabel{"
        "   color: rgb(164, 164, 164);"
        "}"
    );

    this->progressTextLayout->addWidget(this->progressLabel);
    // this->progressTextLayout->addStretch();
    // this->progressTextLayout->addWidget(this->timeLabel);

    this->downloadProgressBar = new QProgressBar();
    this->downloadProgressBar->setValue(60);
    this->downloadProgressBar->setTextVisible(false);
    this->downloadProgressBar->setFixedHeight(3);

    this->downloadProgressBar->setStyleSheet(
        "QProgressBar{"
        "   border: none;"
        "   background-color: rgb(50, 50, 50);"
        "   border-radius: 10px;"
        "}"
        "QProgressBar::chunk{"
        "   background-color: rgb(56, 241, 52);"
        "}"
    );

    this->subLayout->addWidget(this->fileName);
    this->subLayout->addLayout(this->progressTextLayout);
    this->subLayout->addWidget(this->downloadProgressBar);

    this->cancelButton = new IconButton(":/icons/white/close.png");
    this->cancelButton->setFixedSize(30, 30);
    // this->cancelButton->setIconSize(QSize(20, 20));
    this->openFileExplorerButton = new IconButton(":/icons/white/folder-up.png");
    this->openFileExplorerButton->setFixedSize(30, 30);
    // this->openFileExplorerButton->setIconSize(QSize(20, 20));

    this->cancelButton->setStyleSheet(
        "QPushButton{"
        "   padding: 10px;"
        "   background-color: rgb(50, 50, 50);"
        "   border-radius: 10px;"
        "}"
    );

    this->openFileExplorerButton->setStyleSheet(
        "QPushButton{"
        "   padding: 10px;"
        "   background-color: rgb(50, 50, 50);"
        "   border-radius: 10px;"
        "}"
    );

    this->layout->addLayout(this->subLayout);
    this->layout->addWidget(this->cancelButton);
    this->layout->addWidget(this->openFileExplorerButton);
}

void DownloadItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(30, 30, 30, 80)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);
}

DownloadItem::~DownloadItem() = default;

DownloadManager::DownloadManager(QWidget *parent): QDialog(parent){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedWidth(300);

    this->layout = new QVBoxLayout(this);
    this->downloadItems = QList<DownloadItem*>();

    DownloadItem *item = new DownloadItem();
    DownloadItem *item1 = new DownloadItem();
    this->layout->addWidget(item);
    this->layout->addWidget(item1);
}

void DownloadManager::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(30, 30, 30, 190)));
    painter.setPen(QPen(QColor(128, 128, 128)));

    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);
}

void DownloadManager::open(){
    this->move(this->parentWidget()->mapToGlobal(QPoint(10, 30)));
    QDialog::open();
    #ifdef __linux__
        QPainterPath path;
        path.addRoundedRect(rect(), 10, 10);
        KWindowEffects::enableBlurBehind(this->windowHandle(), true, QRegion(path.toFillPolygon().toPolygon()));
    #elif defined(_WIN32)
        enableBlurBehind();
    #endif
}

#ifdef _WIN32
void DownloadManager::enableBlurBehind()
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

DownloadManager::~DownloadManager() = default;
