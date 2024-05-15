#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QMainWindow>
#include "../browserwindow.h"

namespace TitleBarButtons{
    class CloseButton : public QWidget {
        Q_OBJECT
    private:
        BrowserWindow *window;
    public:
        explicit CloseButton(BrowserWindow *window, QWidget *parent = nullptr);
        ~CloseButton();
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
    };

    class MinimizeButton : public QWidget {
        Q_OBJECT
    private:
        BrowserWindow *window;
    public:
        explicit MinimizeButton(BrowserWindow *window, QWidget *parent = nullptr);
        ~MinimizeButton();
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
    };

    class MaximizeButton : public QWidget {
        Q_OBJECT
    private:
        BrowserWindow *window;
    public:
        explicit MaximizeButton(BrowserWindow *window, QWidget *parent = nullptr);
        ~MaximizeButton();
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
    };
}