#include "WebViewDialog.h"

#include <QWebEnginePage>

WebViewDialog::WebViewDialog(QWebEngineProfile *profile, QWidget *parent): QDialog(parent){
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(0, 0, 0, 0);

    this->webView = new WebView(profile);

    this->connect(this->webView, &QWebEngineView::loadFinished, this, [=](){
        this->setWindowTitle(this->webView->title());
    });

    this->connect(this->webView->page(), &QWebEnginePage::windowCloseRequested, this, [=](){
        this->accept();
    });

    this->layout->addWidget(this->webView);
}

WebViewDialog::WebViewDialog(QWebEngineProfile *profile, QUrl url, QWidget *parent): WebViewDialog(profile, parent){
    this->webView->load(url);
}

WebView* WebViewDialog::getView(){
    return this->webView;
}

WebViewDialog::~WebViewDialog() = default;