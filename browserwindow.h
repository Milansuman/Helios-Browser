#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QMainWindow>

// QT_BEGIN_NAMESPACE
// namespace Ui {
//     class BrowserWindow;
// }
// QT_END_NAMESPACE

class BrowserWindow : public QMainWindow {
    Q_OBJECT
private:
    bool isEdgePosition(QPointF position);
    QFlags<Qt::Edge> getEdgePosition(QPointF position);
public:
    bool maximized = false;
    explicit BrowserWindow(QWidget *parent = nullptr, double width=800.0, double height=600.0);
    ~BrowserWindow();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};