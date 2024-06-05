#include "AddressBox.h"

#include <QPainter>
#include <QFontDatabase>
#include <QFont>

AddressBox::AddressBox(QString text, QWidget *parent) : QLabel(parent), isBlank(true) {
    this->setText(text);
    this->setMinimumSize(140, 20);
    this->setFixedHeight(20);
    this->setAlignment(Qt::AlignCenter);

    this->setStyleSheet("color: #888888;");

    QFontDatabase::addApplicationFont(":/fonts/calsans.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont *font = new QFont(fontFamily);

    this->setFont(*font);
}

void AddressBox::paintEvent(QPaintEvent *event){
    if(this->isBlank){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setBrush(QBrush(QColor(58, 58, 58)));
        painter.setPen(Qt::NoPen);

        painter.drawRoundedRect(QRect(0, 0, this->width(), this->height()), 5, 5);
    }

    QLabel::paintEvent(event);
}

void AddressBox::setBlank(bool blank){
    this->isBlank = blank;
}