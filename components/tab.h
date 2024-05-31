#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QResizeEvent>
#include "webview.h"
#include "addressbox.h"

class TabTitleBar : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout* titlebarLayout;
    AddressBox* addressBox;
public:
    TabTitleBar(QWidget *parent=nullptr);
    void setTitle(QString title);
    ~TabTitleBar();
};

class Tab : public QWidget {
    Q_OBJECT
private:
    WebView *webview;
    QVBoxLayout *tabLayout;
    TabTitleBar *titlebar;
    bool isTitleBarShowing;
public:
    Tab(QString url, bool showTitleBar=false, QWidget *parent=nullptr);
    ~Tab();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};