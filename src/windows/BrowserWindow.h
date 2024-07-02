#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPaintEvent>
#include <QRegion>
#include <QPropertyAnimation>

#include "../components/WindowTitleBar.h"
#include "../components/TabManager.h"
#include "../components/SideBar.h"

class BrowserWindow : public QMainWindow {
    Q_OBJECT
private:
    WindowTitleBar *titleBar;
    TabManager *tabManager;
    QVBoxLayout *layout;
    QHBoxLayout *contentLayout;
    QWidget *centralWidget;
    SideBar *sideBar;
    bool isMaximized;
    QPropertyAnimation *sideBarAnimation;

    void hideSideBar();
    bool isEdgePosition(QPointF position);
    QFlags<Qt::Edge> getEdgePosition(QPointF position);
public:
    BrowserWindow(QSize size, QWidget *parent=nullptr);
    BrowserWindow(QUrl url, QSize size, QWidget *parent=nullptr);
    void toggleSideBar();

    #ifdef __linux__
    void show();
    #endif

    ~BrowserWindow();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    #ifdef __linux__
    void resizeEvent(QResizeEvent *event) override;
    #endif
};
