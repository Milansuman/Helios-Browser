#include "DownloadManager.h"

#include <QFontDatabase>
#include <QFont>
#include <QListWidgetItem>
#include <QProgressBar>
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>

DownloadManager::DownloadManager(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("Downloads");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedWidth(300);

    QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont *font = new QFont(fontFamily, -1, QFont::Bold);

    this->dialogLayout = new QVBoxLayout(this);
    this->dialogLayout->setContentsMargins(15, 15, 15, 15);

    downloadList = new QListWidget(this);
    downloadList->setMinimumHeight(200);
    this->dialogLayout->addWidget(downloadList);

    connect(downloadList, &QListWidget::itemDoubleClicked, this, &DownloadManager::openFileLocation);

    // addDownload(QDir::homePath() + "/test_download.txt", 50);
    loadDownloads();
    // this->dialogLayout->addWidget(this->mainPage);
}

void DownloadManager::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(30, 30, 30, 230)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
}

void DownloadManager::saveDownloads()
{
    QFile file(QDir::homePath() + "/.downloads_history");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString &filename : downloads.keys()) {
            out << filename << "\n";
        }
        file.close();
    }
}

void DownloadManager::loadDownloads()
{
    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QDir downloadsDir(downloadsPath);
    
    qDebug() << "Attempting to load downloads from:" << downloadsPath;

    if (downloadsDir.exists()) {
        QFileInfoList files = downloadsDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
        
        for (const QFileInfo &fileInfo : files) {
            QString filename = fileInfo.filePath();
            qDebug() << "Found file:" << filename;
            addDownload(filename, 100);  // Add with 100% progress
        }
    } else {
        qDebug() << "Downloads directory does not exist";
    }
}

void DownloadManager::addDownload(const QString &filename, int progress)
{
    qDebug() << "Adding download:" << filename << "Progress:" << progress;
    if (!downloads.contains(filename)) {
        QListWidgetItem *item = new QListWidgetItem(downloadList);
        downloads[filename] = item;

        QWidget *itemWidget = new QWidget;
        QVBoxLayout *itemLayout = new QVBoxLayout(itemWidget);

        QLabel *filenameLabel = new QLabel(QFileInfo(filename).fileName());
        QProgressBar *progressBar = new QProgressBar;
        progressBar->setValue(progress);

        itemLayout->addWidget(filenameLabel);
        itemLayout->addWidget(progressBar);

        item->setSizeHint(itemWidget->sizeHint());
        downloadList->setItemWidget(item, itemWidget);
    }
    updateDownloadProgress(filename, progress);
}

void DownloadManager::updateDownloadProgress(const QString &filename, int progress)
{
    if (downloads.contains(filename)) {
        QListWidgetItem *item = downloads[filename];
        QWidget *itemWidget = downloadList->itemWidget(item);
        QProgressBar *progressBar = itemWidget->findChild<QProgressBar*>();
        if (progressBar) {
            progressBar->setValue(progress);
        }
    }
}

void DownloadManager::openFileLocation(QListWidgetItem *item)
{
    QString filename = downloads.key(item);
    if (!filename.isEmpty()) {
        QFileInfo fileInfo(filename);
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
    }
}

void DownloadManager::open()
{
    this->move(this->parentWidget()->mapToGlobal(QPoint(this->parentWidget()->width() / 10 - this->width() / 2, 10)));
    QDialog::open();
}

DownloadManager::~DownloadManager() = default;