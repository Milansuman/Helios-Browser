#include "splitTabDialog.h"
#include <QPainter>

SplitTabDialog::SplitTabDialog(QWidget *parent): QDialog(parent){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(30, 30, 30));
    setPalette(palette);

    this->splitLeftButton = new IconButton(":/icons/split-left.png");
    this->splitLeftButton->scale(25, 25);

    connect(this->splitLeftButton, &IconButton::clicked, this, [=](){
        emit this->splitLeftRequested();
    });

    this->splitRightButton = new IconButton(":/icons/split-right.png");
    this->splitRightButton->scale(25, 25);

    connect(this->splitRightButton, &IconButton::clicked, this, [=](){
        emit this->splitRightRequested();
    });

    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(5, 5, 5, 5);
    this->layout->addWidget(this->splitLeftButton);
    this->layout->addWidget(this->splitRightButton);
}

void SplitTabDialog::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    painter.setBrush(QBrush(QColor(30, 30, 30)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
    QDialog::paintEvent(event);
}

void SplitTabDialog::open(){
    this->move(static_cast<QWidget*>(this->parent())->geometry().bottomLeft() + QPoint(-5, 10));
    QDialog::open();
}

SplitTabDialog::~SplitTabDialog(){
    delete this->splitLeftButton;
    delete this->splitRightButton;
}