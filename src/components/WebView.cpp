#include "WebView.h"

#include <QWebEngineView>
#include <QWebEngineProfile>

WebView::WebView(QWebEngineProfile *profile, QWidget *parent): QWebEngineView(profile, parent){
    this->page()->setBackgroundColor(QColor(30, 30, 30));
    this->setAttribute(Qt::WA_OpaquePaintEvent);
}