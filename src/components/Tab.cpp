#include "Tab.h"

#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QClipboard>

Tab::Tab(QWebEngineProfile *profile, QWidget *parent): Tab(profile, "https://browser-homepage-alpha.vercel.app/", parent){}

Tab::Tab(QWebEngineProfile *profile, QString url, QWidget *parent): QWidget(parent){
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(0,0,0,0);
    this->layout->setSpacing(0);

    this->webview = new WebView(profile);
    this->webview->load(QUrl(url));

    this->searchDialog = new SearchDialog(this);

    this->tabTitleBar = new TabTitleBar();

    this->connect(this->webview, &WebView::loadStarted, this, [=](){
        this->tabTitleBar->setTitle(this->webview->url().toString());
    });

    this->connect(this->webview, &WebView::loadFinished, this, [=](){
        this->tabTitleBar->setTitle(this->webview->title());
        emit this->titleChanged(this->webview->title());
    });

    this->connect(this->webview, &WebView::iconChanged, this, [=](){
        emit this->iconChanged(this->webview->icon());
    });

    this->connect(this->tabTitleBar, &TabTitleBar::searchRequested, this, [=](){
        this->searchDialog->open();
    });

    this->connect(this->searchDialog, &SearchDialog::accepted, this, [=](){
        this->webview->load(QUrl(this->searchDialog->getSearch()));
    });

    this->connect(this->tabTitleBar, &TabTitleBar::copyLinkRequested, this, [=](){
        qApp->clipboard()->setText(this->webview->url().toString());
    });

    this->connect(this->tabTitleBar, &TabTitleBar::previousPageRequested, this, [=](){
        this->webview->back();
    });

    this->connect(this->tabTitleBar, &TabTitleBar::nextPageRequested, this, [=](){
        this->webview->forward();
    });

    this->connect(this->tabTitleBar, &TabTitleBar::reloadRequested, this, [=](){
        this->webview->reload();
    });

    this->connect(this->tabTitleBar, &TabTitleBar::splitTabLeftRequested, this, [=](){
        emit this->splitTabLeftRequested();
    });

    this->connect(this->tabTitleBar, &TabTitleBar::splitTabRightRequested, this, [=](){
        emit this->splitTabRightRequested();
    });

    this->connect(this->tabTitleBar, &TabTitleBar::closeTabRequested, this, [=](){
        emit this->closeTabRequested();
    });

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

void Tab::setTitleBarVisible(bool visible){
    this->tabTitleBar->setVisible(visible);
}

void Tab::requestSearchDialog(){
    this->searchDialog->open();
}

QString Tab::getTitle(){
    return this->webview->title();
}

QIcon Tab::getIcon(){
    return this->webview->icon();
}

void Tab::copyUrl(){
    qApp->clipboard()->setText(this->webview->url().toString());
}

void Tab::requestNextPage(){
    this->webview->forward();
}

void Tab::requestPreviousPage(){
    this->webview->back();
}

void Tab::requestReload(){
    this->webview->reload();
}

Tab::~Tab(){
    delete this->webview;
    delete this->tabTitleBar;
    delete this->layout;
    delete this->searchDialog;
}