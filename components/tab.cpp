#include "tab.h"
#include "addressbox.h"
#include "tabTitleBar.h"

#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>

Tab::Tab(QWebEngineProfile *profile, bool showTitleBar, QWidget *parent): Tab(profile, "https://browser-homepage-alpha.vercel.app/", showTitleBar, parent){}

Tab::Tab(QWebEngineProfile *profile, QString url, bool showTitleBar, QWidget *parent): QWidget(parent), isTitleBarShowing(showTitleBar){
    this->setMinimumWidth(250);
    this->setMouseTracking(true);
    this->webview = new WebView(profile);
    this->webview->load(QUrl(url));

    this->tabLayout = new QVBoxLayout();

    this->titlebar = new TabTitleBar();

    connect(this->webview, &WebView::loadFinished, this, [=](){
        this->titlebar->setTitle(this->webview->title());
    });

    connect(this->titlebar, &TabTitleBar::search, this, [=](QString search){
        this->webview->load(QUrl(search));
    });

    this->titlebar->setVisible(this->isTitleBarShowing);
    
    this->tabLayout->addWidget(this->titlebar);
    this->tabLayout->addWidget(this->webview);

    this->tabLayout->setSpacing(0);
    this->tabLayout->setContentsMargins(0,0,0,0);
    this->setLayout(this->tabLayout);
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

void Tab::mousePressEvent(QMouseEvent *event){
    qDebug() << "tab focused";
}

void Tab::setTitleBarVisible(bool visible){
    this->isTitleBarShowing = visible;
    this->titlebar->setVisible(visible);
}

TabTitleBar* Tab::getTitleBar(){
    return this->titlebar;
}

Tab::~Tab(){
    delete this->webview;
    delete this->tabLayout;
    delete this->titlebar;
}