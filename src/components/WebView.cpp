#include "WebView.h"

#include <QWebEngineView>
#include <QWebEngineProfile>

WebView::WebView(QWebEngineProfile *profile, QWidget *parent): QWebEngineView(profile, parent){}