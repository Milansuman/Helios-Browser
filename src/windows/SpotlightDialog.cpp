#include "SpotlightDialog.h"

#include <QPainterPath>
#include <QPainter>

SpotlightDialog::SpotlightDialog(QWidget *parent): QDialog(parent){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setFixedSize(parent->size());
    qDebug() << this->size();
    this->layout = new QVBoxLayout(this);

    this->webview = new WebView();
    this->webview->page()->setBackgroundColor(QColor(0, 0, 0, 0));

    this->webview->load(QUrl("https://jovit-mathew236.github.io/project-web-spotlight/"));

    this->layout->addWidget(this->webview);
}

void SpotlightDialog::open(int pos, int group){
    this->pos = pos;
    this->group = group;
    qDebug() << "test";
    QDialog::open();
}

SpotlightDialog::~SpotlightDialog() = default;