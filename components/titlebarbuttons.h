#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QMainWindow>

namespace TitleBarButtons{
    class CloseButton : public QWidget {
        Q_OBJECT
    private:
        QMainWindow *window;
    public:
        explicit CloseButton(QMainWindow *window, QWidget *parent = nullptr);
        ~CloseButton();
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
    };

    class MinimizeButton : public QWidget {
        Q_OBJECT
    private:
        QMainWindow *window;
    public:
        explicit MinimizeButton(QMainWindow *window, QWidget *parent = nullptr);
        ~MinimizeButton();
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
    };

    class MaximizeButton : public QWidget {
        Q_OBJECT
    private:
        QMainWindow *window;
        bool isMaximized = false;
    public:
        explicit MaximizeButton(QMainWindow *window, QWidget *parent = nullptr);
        ~MaximizeButton();
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
    };
}