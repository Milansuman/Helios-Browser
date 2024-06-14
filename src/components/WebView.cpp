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

