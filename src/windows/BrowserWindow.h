#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QPaintEvent>

#include "../components/WindowTitleBar.h"

class BrowserWindow : public QMainWindow {
    Q_OBJECT
private:
    WindowTitleBar *titleBar;
    QVBoxLayout *layout;
    QWidget *centralWidget;
    bool isMaximized;

    bool isEdgePosition(QPointF position);
    QFlags<Qt::Edge> getEdgePosition(QPointF position);
public:
    BrowserWindow(QSize size, QWidget *parent=nullptr);
    ~BrowserWindow();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
};