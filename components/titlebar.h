#pragma once

#include <QWidget>
#include <QMouseEvent>
#include "../browserwindow.h"

class TitleBar : public QWidget {
    Q_OBJECT
private:
    bool moving;
    QPointF originalPosition;
    QRect originalGeometry;
    BrowserWindow *window;
public:
    TitleBar(BrowserWindow *window, QWidget *parent = nullptr);
    ~TitleBar();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};