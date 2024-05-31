#include "tab.h"
#include "addressbox.h"

#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>

TabTitleBar::TabTitleBar(QWidget *parent): QWidget(parent){
    this->setFixedHeight(25);
    this->titlebarLayout = new QHBoxLayout(this);

    this->addressBox = new AddressBox("");
    this->addressBox->setBlank(false);

    this->titlebarLayout->addStretch();
    this->titlebarLayout->addWidget(this->addressBox);
    this->titlebarLayout->addStretch();
}

void TabTitleBar::setTitle(QString title){
    this->addressBox->setText(title);
}

TabTitleBar::~TabTitleBar(){
    delete this->titlebarLayout;
    delete this->addressBox;
}

Tab::Tab(QString url, bool showTitleBar, QWidget *parent): QWidget(parent), isTitleBarShowing(showTitleBar){
    this->webview = new WebView();
    this->webview->load(QUrl(url));

    this->tabLayout = new QVBoxLayout();

    if(this->isTitleBarShowing){
        this->titlebar = new TabTitleBar();

        QObject::connect(this->webview, &WebView::loadFinished, this, [=](){
            this->titlebar->setTitle(this->webview->title());
        });
        
        this->tabLayout->addWidget(this->titlebar);
    }
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

Tab::~Tab(){
    delete this->webview;
    delete this->tabLayout;
    delete this->titlebar;
}