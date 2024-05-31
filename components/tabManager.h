#pragma once

#include <vector>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QWidget>
#include "webview.h"

// struct TabGroup{
//     std::vector<WebView*> webviews;
//     QWidget *canvas;
//     QHBoxLayout *layout;
// };

class TabManager : public QStackedWidget {
    Q_OBJECT
private:
    //Tabs could contain groups of tabs
    WebView* currentWebView = nullptr;
public:
    TabManager(QWidget *parent=nullptr);
    //void updateLayout();
    ~TabManager();
};