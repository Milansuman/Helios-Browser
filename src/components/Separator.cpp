#include "Separator.h"

#include <QPainter>

Separator::Separator(QWidget *parent): QWidget(parent){
    this->setFixedHeight(2);
}

void Separator::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(76, 76, 76)));
    painter.setPen(Qt::NoPen);

    painter.drawRect(rect());
}

Separator::~Separator() = default;