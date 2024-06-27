#pragma once

#include <QWidget>
#include <QWebEngineProfile>
#include <QWebEnginePage>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QIcon>
#include <QLabel>
#include <QSplitter>
#include <vector>
#include <QUrl>

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
    std::vector<QWebEnginePage::Feature> *permissions; //bitwise or permissions flags

    void initCustomScrollBar();
    bool hasPermission(QWebEnginePage::Feature permission);
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
    void load(QUrl url);
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
    void newTabRequested(QUrl url);
};
