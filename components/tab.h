#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include "webview.h"
#include "tabTitleBar.h"
#include "searchDialog.h"

class Tab : public QWidget {
    Q_OBJECT
private:
    WebView *webview;
    QVBoxLayout *tabLayout;
    TabTitleBar *titlebar;
    bool isTitleBarShowing;
public:
    //show homepage if url is not provided
    Tab(QWebEngineProfile *profile, bool showTitleBar=false, QWidget *parent=nullptr);
    Tab(QWebEngineProfile *profile, QString url, bool showTitleBar=false, QWidget *parent=nullptr);
    void setTitleBarVisible(bool visible);
    TabTitleBar* getTitleBar();
    void insertTitleBar(TabTitleBar *titlebar);
    ~Tab();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals: 
    void splitLeftRequested();
    void splitRightRequested();
};