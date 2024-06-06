#include "Tab.h"

#include <QPainter>
#include <QPainterPath>

Tab::Tab(QWebEngineProfile *profile, QWidget *parent): Tab(profile, "https://browser-homepage-alpha.vercel.app/", parent){}

Tab::Tab(QWebEngineProfile *profile, QString url, QWidget *parent): QWidget(parent){
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(0,0,0,0);
    this->layout->setSpacing(0);

    this->webview = new WebView(profile);
    this->webview->load(QUrl(url));

    this->tabTitleBar = new TabTitleBar();

    this->layout->addWidget(this->tabTitleBar);
    this->layout->addWidget(this->webview);
}

void Tab::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(0, 0, 0)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
}

void Tab::resizeEvent(QResizeEvent *event){
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    this->setMask(path.toFillPolygon().toPolygon());
}

Tab::~Tab(){
    delete this->webview;
    delete this->tabTitleBar;
    delete this->layout;
}