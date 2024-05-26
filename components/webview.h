#pragma once

#include <QWebEngineView>

class WebView : public QWebEngineView{
    Q_OBJECT
public:
    WebView(QWidget *parent = nullptr);
};