#include "webview.h"

#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QApplication>

WebView::WebView(QWebEngineProfile *profile, QWidget *parent): QWebEngineView(profile, parent){
    QApplication::instance()->installEventFilter(this);
    this->setMouseTracking(true);
}

void WebView::focusInEvent(QFocusEvent *event){
    emit this->recievedFocus();
    QWebEngineView::focusInEvent(event);
}

bool WebView::eventFilter(QObject *object, QEvent *event){
    if(object->parent() == this){
        switch (event->type()){
        case QEvent::MouseMove:
            this->mouseMoveEvent(static_cast<QMouseEvent *>(event));
            break;
        case QEvent::FocusIn:
            this->focusInEvent(static_cast<QFocusEvent*>(event));
            break;
        default:
            break;
        }
    }

    return false;
}