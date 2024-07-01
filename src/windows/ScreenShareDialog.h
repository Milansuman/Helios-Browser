#pragma once

#include <QDialog>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QWebEngineDesktopMediaRequest>
#include <QModelIndex>
#include <vector>

#include "../components/PreviewWindow.h"

class ScreenShareDialog : public QDialog{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    QTabWidget *optionsWidget;
    QGridLayout *screensLayout, *windowsLayout;
    QWidget *screensWidget, *windowsWidget;
    std::vector<PreviewWindow*> screens;
    std::vector<PreviewWindow*> windows;
    const QWebEngineDesktopMediaRequest *request;

    void addScreen(QModelIndex screen);
    void addWindow(QModelIndex window);
public:
    ScreenShareDialog(QWidget *parent=nullptr);
    void exec(const QWebEngineDesktopMediaRequest &request);
    ~ScreenShareDialog();
protected:
    void paintEvent(QPaintEvent *event) override;
};