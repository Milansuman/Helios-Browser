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
    WebViewDialog(WebView *webview, QWidget *parent=nullptr);
    ~WebViewDialog();
};