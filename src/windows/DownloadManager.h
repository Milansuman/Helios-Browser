#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QProgressBar>
#include <QMap>
#include <QLabel>
#include <QWebEngineDownloadRequest>

#include "../components/IconButton.h"

class DownloadItem: public QWidget{
    Q_OBJECT
private:
    QHBoxLayout *layout, *progressTextLayout;
    QVBoxLayout *subLayout;
    QLabel *fileName, *progressLabel, *timeLabel;
    QProgressBar *downloadProgressBar;
    IconButton *cancelButton, *openFileExplorerButton, *pauseButton, *resumeButton;

    qint64 totalSize, downloadedSize;
    bool downloadComplete, isPaused;
public:
    DownloadItem(QString filename, qint64 totalSize, QWidget *parent=nullptr);
    void setDownloaded(qint64 size, qint64 totalSize);
    void setCompleted();
    ~DownloadItem();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void pauseDownload();
    void resumeDownload();
    void stopDownload();
    void openFolder();
    void cancelDownload();
    void clicked();
};

class DownloadManager: public QDialog{
    Q_OBJECT
private:
    QMap<QWebEngineDownloadRequest*, DownloadItem*> downloadItems;
    QVBoxLayout *layout;

    #ifdef _WIN32
    void enableBlurBehind();
    #endif
public:
    DownloadManager(QWidget *parent=nullptr);
    void open();
    void addDownloadItem(QWebEngineDownloadRequest *request);
    void updateDownloadItem(QWebEngineDownloadRequest *request);
    void finishDownloadItem(QWebEngineDownloadRequest *request);
    ~DownloadManager();
protected:
    void paintEvent(QPaintEvent *event);
};
