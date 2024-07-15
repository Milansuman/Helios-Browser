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
#include <QWidgetAction>
#include "../components/IconButton.h"

class DownloadManager : public QDialog
{
    Q_OBJECT
private:
    // QLabel *messageLabel;
    QStackedLayout *layout;
    QVBoxLayout *dialogLayout;
    QListWidget *downloadList;
    QWidget *mainPage;
    QMenu *menu;

public:
    DownloadManager(QWidget *parent = nullptr);
    void addDownload(const QString &filename, int progress = 0);
    void updateDownloadProgress(const QString &filename, int progress);
    void open();
    ~DownloadManager();

protected:
    void paintEvent(QPaintEvent *event) override;
};