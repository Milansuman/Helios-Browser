#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QProgressBar>
#include <QList>
#include <QLabel>

#include "../components/IconButton.h"

class DownloadItem: public QWidget{
    Q_OBJECT
private:
    QHBoxLayout *layout, *progressTextLayout;
    QVBoxLayout *subLayout;
    QLabel *fileName, *progressLabel, *timeLabel;
    QProgressBar *downloadProgressBar;
    IconButton *cancelButton, *openFileExplorerButton;
public:
    DownloadItem(QWidget *parent=nullptr);
    ~DownloadItem();
protected:
    void paintEvent(QPaintEvent *event) override;
};

class DownloadManager: public QDialog{
    Q_OBJECT
private:
    QList<DownloadItem*> downloadItems;
    QVBoxLayout *layout;
public:
    DownloadManager(QWidget *parent=nullptr);
    void open();
    ~DownloadManager();
protected:
    void paintEvent(QPaintEvent *event);
};
