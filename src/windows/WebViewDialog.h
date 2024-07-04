#pragma once

#include <QDialog>
#include <QWebEngineProfile>
#include <QVBoxLayout>

#include "../components/WebView.h"

class WebViewDialog : public QDialog{
    Q_OBJECT
private:
    WebView *webView;
    QVBoxLayout *layout;
public:
    WebViewDialog(QWebEngineProfile *profile, QWidget *parent=nullptr);
    WebViewDialog(QWebEngineProfile *profile, QUrl url, QWidget *parent=nullptr);
    WebView* getView();
    ~WebViewDialog();
};