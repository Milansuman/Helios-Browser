#pragma once

#include <QWidget>
#include <QMouseEvent>

// QT_BEGIN_NAMESPACE
// namespace Ui {
//     class BrowserWindow;
// }
// QT_END_NAMESPACE

enum WindowBoundary {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    NOT_BOUNDARY
};

class BrowserWindow : public QWidget {
    Q_OBJECT
private:
    //==================HANDLING WINDOW RESIZE FUNCTIONALITY==================================
    bool resizing;
    WindowBoundary currentEdgePosition;
    QRect originalGeometry;
    QPointF lastMousePosition;
    bool isEdgePosition(QPointF position);
    WindowBoundary edgePosition(QPointF position);
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