#pragma once

#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QPaintEvent>
#include <QEvent>
#include <QQuickWidget>

class WebView : public QWebEngineView{
    Q_OBJECT
private:
    QQuickWidget *pageSurface;
    QColor getTopColor();
public:
    WebView(QWebEngineProfile *profile, QWidget *parent = nullptr);
    WebView(QWidget *parent=nullptr);
signals:
    void colorChanged(QColor color);
};