#include "PreviewWindow.h"

#include <QPainter>

PreviewWindow::PreviewWindow(QString title, QPixmap *preview, QWidget *parent): QWidget(parent){
    this->layout = new QVBoxLayout(this);
    this->setFixedSize(200, 150);

    this->titlebar = new QWidget();
    this->titleBarLayout = new QHBoxLayout(this->titlebar);

    this->title = new QLabel(title);

    this->titleBarLayout->addStretch();
    this->titleBarLayout->addWidget(this->title);
    this->titleBarLayout->addStretch();

    this->layout->addWidget(this->titlebar);

    if(preview){
        this->preview = new QLabel();
        this->preview->setPixmap(*preview);
        this->layout->addWidget(this->preview);
    }else{
        this->layout->addStretch();
    }
}

void PreviewWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(0,0,0)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
}

void PreviewWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit this->clicked();
    }
}

PreviewWindow::~PreviewWindow() = default;