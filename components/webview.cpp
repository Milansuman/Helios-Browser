#include "webview.h"

#include <QWebEngineView>
#include <QWebEngineSettings>

WebView::WebView(QWidget *parent): QWebEngineView(parent){
    QWebEngineSettings *settings = this->page()->settings();
    settings->setAttribute(QWebEngineSettings::WebAttribute::AutoLoadImages, true);
    settings->setAttribute(QWebEngineSettings::WebAttribute::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::WebAttribute::Accelerated2dCanvasEnabled, true);
    settings->setAttribute(QWebEngineSettings::WebAttribute::WebGLEnabled, true);
    settings->setAttribute(QWebEngineSettings::WebAttribute::FullScreenSupportEnabled, true);
}