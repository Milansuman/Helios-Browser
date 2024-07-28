#pragma once

#include <QDialog>
#include <QPaintEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QStackedLayout>
#include <QMenu>
#include <QCloseEvent>
#include <QMap>
#include <QWidgetAction>
#include "../components/IconButton.h"

class DownloadManager : public QDialog
{
    Q_OBJECT
private:
    // QLabel *messageLabel;
    QVBoxLayout *dialogLayout;
    QListWidget *downloadList;
    QMap<QString, QListWidgetItem*> downloads;
    QWidget *mainPage;
    QMenu *menu;

    void openFileLocation(QListWidgetItem *item);

public:
    // DownloadManager(QWidget *parent = nullptr);
    explicit DownloadManager(QWidget *parent = nullptr);
    void addDownload(const QString &filename, int progress = 0);
    void updateDownloadProgress(const QString &filename, int progress);
    void open();
    void saveDownloads();   
    void loadDownloads();
    
    ~DownloadManager();

protected:
    void paintEvent(QPaintEvent *event) override;
    // void closeEvent(QCloseEvent *event) override;
};