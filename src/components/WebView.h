#pragma once

#include <QWebEngineView>
#include <QWebEngineProfile>

class WebView : public QWebEngineView{
    Q_OBJECT
public:
    WebView(QWebEngineProfile *profile, QWidget *parent = nullptr);
    WebView(QWidget *parent=nullptr);
};