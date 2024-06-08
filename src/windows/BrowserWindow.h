#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QPaintEvent>
#ifdef _WIN32
#include <windows.h>
#endif

#include "../components/WindowTitleBar.h"
#include "../components/TabManager.h"

class BrowserWindow : public QMainWindow {
    Q_OBJECT
private:
    WindowTitleBar *titleBar;
    TabManager *tabManager;
    QVBoxLayout *layout;
    QWidget *centralWidget;
    bool isMaximized;
#ifdef _WIN32
    HWND windowID;
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

    bool isEdgePosition(QPointF position);
    QFlags<Qt::Edge> getEdgePosition(QPointF position);
public:
    BrowserWindow(QSize size, QWidget *parent=nullptr);
    ~BrowserWindow();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};
