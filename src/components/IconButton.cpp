#include "IconButton.h"

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

IconButton::~IconButton(){
    delete this->icon;
}