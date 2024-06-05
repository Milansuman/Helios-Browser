#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

class WindowTitleBar : public QWidget{
    Q_OBJECT
private:
    QHBoxLayout *tabTitleBarLayout, *windowButtonsLayout;
    QPushButton *minimize, *maximize, *close;
public:
    WindowTitleBar(QWidget *parent=nullptr);
    QPushButton* minimizeButton();
    QPushButton* maximizeButton();
    QPushButton* closeButton();
    ~WindowTitleBar();
};