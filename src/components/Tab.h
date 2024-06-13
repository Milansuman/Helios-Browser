#pragma once

#include <QWidget>
#include <QWebEngineProfile>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QIcon>

#include "WebView.h"
#include "TabTitleBar.h"
#include "../windows/SearchDialog.h"
#include "../windows/FullScreenWindow.h"

class Tab : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout *layout;
    WebView *webview;
    TabTitleBar *tabTitleBar;
    SearchDialog *searchDialog;
    FullScreenWindow *fullScreenWindow;


public:
    Tab(QWebEngineProfile *profile, QWidget *parent=nullptr);
    Tab(QWebEngineProfile *profile, QString url, QWidget *parent=nullptr);
    void setTitleBarVisible(bool visible);
    void requestSearchDialog();
    QString getTitle();
    QIcon getIcon();
    void copyUrl();
    void requestNextPage();
    void requestPreviousPage();
    void requestReload();
    ~Tab();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
signals:
    void closeTabRequested();
    void splitTabLeftRequested();
    void splitTabRightRequested();
    void titleChanged(QString title);
    void iconChanged(QIcon icon);
};
