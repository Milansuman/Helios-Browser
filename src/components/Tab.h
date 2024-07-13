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
#include <QUrl>
#include <QProgressBar>
#include <vector>
#include <map>

#include "WebView.h"
#include "TabTitleBar.h"
#include "../windows/SearchDialog.h"
#include "../windows/FullScreenWindow.h"
#include "../windows/AuthenticationDialog.h"
#include "../windows/PermissionDialog.h"
#include "../windows/ScreenShareDialog.h"
#include "../windows/WebViewDialog.h"
#include "../windows/PageSettingsDialog.h"

class Tab : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout *layout;
    WebView *webview, *devtools;
    QLabel *pageSurface;
    TabTitleBar *tabTitleBar;
    QProgressBar *progressIndicator;
    QSplitter *devtoolsSplitter;
    SearchDialog *searchDialog;
    FullScreenWindow *fullScreenWindow;
    AuthenticationDialog *authDialog;
    PermissionDialog *permissionDialog;
    ScreenShareDialog *screenShareDialog;
    PageSettingsDialog *pageSettingsDialog;
    QWebEngineProfile *profile;
    std::map<QWebEnginePage::Feature, bool> *permissions;

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
    void showSiteSettings();
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
    void splitTabFlipRequested();
    void titleChanged(QString title);
    void iconChanged(QIcon icon);
    void tabFocused();
    void newTabRequested(QUrl url);
    void newWindowRequested(QUrl url);
};
