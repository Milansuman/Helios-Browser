#pragma once

#include <QWidget>
#include <QResizeEvent>

#include "../components/WebView.h"

class FullScreenWindow : public QWidget{
    Q_OBJECT
private:
    WebView *fullScreenView, *previousView;
public:
    FullScreenWindow(QWebEngineProfile *profile, WebView *view, QWidget *parent=nullptr);
    ~FullScreenWindow();
protected:
    void resizeEvent(QResizeEvent *event) override;
};