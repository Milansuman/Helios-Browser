#pragma once

#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QPaintEvent>
#include <QEvent>

class WebView : public QWebEngineView{
    Q_OBJECT
private:
    QColor getTopColor();
public:
    WebView(QWebEngineProfile *profile, QWidget *parent = nullptr);
    WebView(QWidget *parent=nullptr);
signals:
    void colorChanged(QColor color);
};