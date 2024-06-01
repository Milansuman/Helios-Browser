#pragma once

#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QFocusEvent>

class WebView : public QWebEngineView{
    Q_OBJECT
public:
    WebView(QWebEngineProfile *profile, QWidget *parent = nullptr);
protected:
    void focusInEvent(QFocusEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event) override;
signals:
    void recievedFocus();
};