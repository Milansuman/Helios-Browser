#include "IconButton.h"

#include <QIcon>

IconButton::IconButton(QString filename, QWidget *parent): QPushButton(parent){
    this->icon = new QPixmap(filename);
    this->icon->scaled(20, 20);
    this->setIcon(QIcon(*this->icon));
    this->setFixedSize(20, 20);

    this->setStyleSheet(
        "QPushButton{"
        "   background: transparent;"
        "   border: none;"
        "}"
    );
}

void IconButton::scale(int w, int h){
    this->icon->scaled(w, h);
    this->setIcon(QIcon(*this->icon));
    this->setFixedSize(w, h);
}

void IconButton::setButtonIcon(QString filename){
    this->icon = new QPixmap(filename);
    this->icon->scaled(this->size());
    this->setIcon(QIcon(*this->icon));
}

IconButton::~IconButton(){
    delete this->icon;
}