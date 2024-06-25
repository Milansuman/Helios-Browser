#pragma once

#include <QWidget>
#include <QWebEngineProfile>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QIcon>
#include <QLabel>
#include <QSplitter>

#include "WebView.h"
#include "TabTitleBar.h"
#include "../windows/SearchDialog.h"
#include "../windows/FullScreenWindow.h"
#include "../windows/AuthenticationDialog.h"
#include "../windows/PermissionDialog.h"

class Tab : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout *layout;
    WebView *webview, *devtools;
    QLabel *pageSurface;
    TabTitleBar *tabTitleBar;
    QSplitter *devtoolsSplitter;
    SearchDialog *searchDialog;
    FullScreenWindow *fullScreenWindow;
    AuthenticationDialog *authDialog;
    PermissionDialog *permissionDialog;
    void initCustomScrollBar();
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
    void openDevTools();
    void closeDevTools();
    ~Tab();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void closeTabRequested();
    void splitTabLeftRequested();
    void splitTabRightRequested();
    void titleChanged(QString title);
    void iconChanged(QIcon icon);
    void tabFocused();
};
