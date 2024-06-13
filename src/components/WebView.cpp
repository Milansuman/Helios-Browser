#include "WebView.h"

#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>

WebView::WebView(QWebEngineProfile *profile, QWidget *parent): QWebEngineView(profile, parent), pageSurface(nullptr){
    this->connect(this, &WebView::loadFinished, this, [=](){
        QTimer::singleShot(200, [=](){
            emit this->colorChanged(this->getTopColor());
        });
    });
}

bool WebView::eventFilter(QObject *object, QEvent *event){
    if(object == this->pageSurface && event->type() == QEvent::Paint){
        qDebug() << "paint event";
        QPaintEvent *paintEvent = static_cast<QPaintEvent*>(event);

        QPainter painter(this->pageSurface);

        painter.drawRoundedRect(this->pageSurface->rect(), 10, 10);

        return true;
    }
    return QWebEngineView::eventFilter(object, event);
}

bool WebView::event(QEvent *event){
    if(event->type() == QEvent::ChildAdded){
        QChildEvent *childEvent = static_cast<QChildEvent*>(event);

        qDebug() << childEvent->child();

        QQuickWidget *webViewWidget = qobject_cast<QQuickWidget*>(childEvent->child());
        if(webViewWidget){
            qDebug() << "found object";
            this->pageSurface = webViewWidget;
            webViewWidget->installEventFilter(this);
        }
    }
    return QWebEngineView::event(event);
}

QColor WebView::getTopColor(){
    QImage page(this->size(), QImage::Format_ARGB32);
    QPainter painter(&page);

    this->render(&painter);
    painter.end();

    int r = 0, g = 0, b = 0;
    int count = 0;

    for (int i = 0; i < 2; ++i) {  // Iterate over the first two rows
        for (int j = 0; j < page.width(); ++j) {
            QColor color = page.pixelColor(j, i);
            r += color.red();
            g += color.green();
            b += color.blue();
            ++count;
        }
    }

    if (count > 0) {
        r /= count;
        g /= count;
        b /= count;
    }

    return QColor(r, g, b);
}

WebView::WebView(QWidget *parent): QWebEngineView(parent){
    this->connect(this, &WebView::loadFinished, this, [=](){
        QTimer::singleShot(200, [=](){
            emit this->colorChanged(this->getTopColor());
        });
    });
}

