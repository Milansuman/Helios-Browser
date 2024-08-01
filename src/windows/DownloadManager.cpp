#include "DownloadManager.h"

#include <QPainter>
#include <QPainterPath>
#include <QFontDatabase>
#include <QFont>
#include <QLocale>
#include <QMouseEvent>
#include <QDesktopServices>

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

DownloadItem::DownloadItem(QString filename, qint64 totalSize, QWidget *parent): QWidget(parent), totalSize(totalSize), downloadComplete(false), isPaused(false){
    int titleFontId = QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    int subTextFontId = QFontDatabase::addApplicationFont(":/fonts/SFUIText-Regular.ttf");

    QFont titleFont(QFontDatabase::applicationFontFamilies(titleFontId).at(0), 8, QFont::Bold);
    QFont subTextFont(QFontDatabase::applicationFontFamilies(subTextFontId).at(0), 8, QFont::Normal);

    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(10, 10, 10, 10);
    this->subLayout = new QVBoxLayout();
    this->subLayout->setSpacing(2);

    this->fileName = new QLabel(filename);
    this->fileName->setFont(titleFont);

    this->progressTextLayout = new QHBoxLayout();
    this->progressLabel = new QLabel("0B/" + QLocale::system().formattedDataSize(this->totalSize));
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
    this->downloadProgressBar->setValue(0);
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

    this->pauseButton = new IconButton(":/icons/white/pause.png");
    this->pauseButton->setFixedSize(30, 30);

    this->resumeButton = new IconButton(":/icons/white/play.png");
    this->resumeButton->setFixedSize(30, 30);

    this->cancelButton->setStyleSheet(
        "QPushButton{"
        "   padding: 10px;"
        "   background-color: rgb(50, 50, 50);"
        "   border-radius: 5px;"
        "}"
    );

    this->openFileExplorerButton->setStyleSheet(
        "QPushButton{"
        "   padding: 10px;"
        "   background-color: rgb(50, 50, 50);"
        "   border-radius: 5px;"
        "}"
    );

    this->pauseButton->setStyleSheet(
        "QPushButton{"
        "   padding: 10px;"
        "   background-color: rgb(50, 50, 50);"
        "   border-radius: 5px;"
        "}"
    );

    this->resumeButton->setStyleSheet(
        "QPushButton{"
        "   padding: 10px;"
        "   background-color: rgb(50, 50, 50);"
        "   border-radius: 5px;"
        "}"
    );

    this->resumeButton->setVisible(false);

    this->connect(this->pauseButton, &IconButton::clicked, this, [=](){
        this->pauseButton->setVisible(false);
        this->resumeButton->setVisible(true);

        emit this->pauseDownload();
    });

    this->connect(this->resumeButton, &IconButton::clicked, this, [=](){
        this->pauseButton->setVisible(true);
        this->resumeButton->setVisible(false);

        emit this->resumeDownload();
    });

    this->connect(this->cancelButton, &IconButton::clicked, this, [=](){
        this->pauseButton->setVisible(false);
        this->resumeButton->setVisible(false);
        this->cancelButton->setVisible(false);
        this->openFileExplorerButton->setVisible(false);
        this->downloadProgressBar->setVisible(false);

        this->progressLabel->setText("Download cancelled.");

        emit this->cancelDownload();
    });

    this->connect(this->openFileExplorerButton, &IconButton::clicked, this, [=](){
        emit this->openFolder();
    });

    this->layout->addLayout(this->subLayout);
    this->layout->addWidget(this->cancelButton);
    this->layout->addWidget(this->pauseButton);
    this->layout->addWidget(this->resumeButton);
    this->layout->addWidget(this->openFileExplorerButton);
}

void DownloadItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(30, 30, 30, 80)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);
}

void DownloadItem::setDownloaded(qint64 size, qint64 totalSize){
    this->downloadedSize = size;
    this->totalSize = totalSize;
    this->progressLabel->setText(QLocale::system().formattedDataSize(size) + "/" + QLocale::system().formattedDataSize(totalSize));

    this->downloadProgressBar->setValue(((double) size/totalSize) * 100);
}

void DownloadItem::setCompleted(){
    this->pauseButton->setVisible(false);
    this->resumeButton->setVisible(false);
    this->cancelButton->setVisible(false);
    this->openFileExplorerButton->setVisible(true);
    this->downloadProgressBar->setVisible(false);

    this->progressLabel->setText("Download completed.");
}

void DownloadItem::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit this->clicked();
    }
}

DownloadItem::~DownloadItem() = default;

DownloadManager::DownloadManager(QWidget *parent): QDialog(parent){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedWidth(300);

    this->layout = new QVBoxLayout(this);
    this->downloadItems = QMap<QWebEngineDownloadRequest*, DownloadItem*>();
}

void DownloadManager::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(30, 30, 30, 190)));
    painter.setPen(QPen(QColor(128, 128, 128)));

    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);
}

void DownloadManager::open(){
    this->move(this->parentWidget()->mapToGlobal(QPoint(10, 10)));
    QDialog::open();
    #ifdef __linux__
        QPainterPath path;
        path.addRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);
        KWindowEffects::enableBlurBehind(this->windowHandle(), true, QRegion(path.toFillPolygon().toPolygon()));
    #elif defined(_WIN32)
        enableBlurBehind();
    #endif
}

void DownloadManager::addDownloadItem(QWebEngineDownloadRequest *request){
    DownloadItem *downloadItem = new DownloadItem(request->suggestedFileName(), request->totalBytes());
    this->downloadItems.insert(request, downloadItem);

    this->connect(downloadItem, &DownloadItem::pauseDownload, this, [=](){
        request->pause();
    });

    this->connect(downloadItem, &DownloadItem::resumeDownload, this, [=](){
        request->resume();
    });

    this->connect(downloadItem, &DownloadItem::cancelDownload, this, [=](){
        request->cancel();
    });

    this->connect(downloadItem, &DownloadItem::openFolder, this, [=](){
        QDesktopServices::openUrl(QUrl(request->downloadDirectory()));
    });

    this->connect(downloadItem, &DownloadItem::clicked, this, [=](){
        QDesktopServices::openUrl(QUrl::fromLocalFile(request->downloadDirectory() + "/" + request->downloadFileName()));
    });

    this->layout->addWidget(downloadItem);
}

void DownloadManager::updateDownloadItem(QWebEngineDownloadRequest *request){
    this->downloadItems[request]->setDownloaded(request->receivedBytes(), request->totalBytes());
}

void DownloadManager::finishDownloadItem(QWebEngineDownloadRequest *request){
    this->downloadItems[request]->setCompleted();
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
