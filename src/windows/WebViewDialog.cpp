#include "WebViewDialog.h"

#include <QWebEnginePage>

WebViewDialog::WebViewDialog(WebView *webView, QWidget *parent): QDialog(parent), webView(webView){
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(0, 0, 0, 0);
    this->setMinimumSize(500, 600);

    this->connect(this->webView, &QWebEngineView::loadFinished, this, [=](){
        this->setWindowTitle(this->webView->title());
    });

    this->connect(this->webView->page(), &QWebEnginePage::windowCloseRequested, this, [=](){
        this->accept();
    });

    this->layout->addWidget(this->webView);
}

WebViewDialog::~WebViewDialog() = default;