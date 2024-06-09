#include "GroupSelectorDialog.h"

#include <QPainter>

GroupSelectorDialog::GroupSelectorDialog(QWidget *parent): QDialog(parent){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setFixedHeight(60);
    this->setMinimumWidth(60);

    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(5, 5, 5, 5);
    this->layout->setSpacing(5);
}

void GroupSelectorDialog::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(26, 29, 30)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 15, 15);
    QDialog::paintEvent(event);
}

void GroupSelectorDialog::addGroup(GroupIcons *group){
    this->groups.push_back(group);
    this->layout->addWidget(group);

    int pos = this->groups.size()-1;

    this->connect(group, &GroupIcons::clicked, this, [=](){
        emit this->changeGroupRequested(pos);
    });
}

void GroupSelectorDialog::insertGroup(int pos, GroupIcons *group){
    this->groups.insert(this->groups.begin()+pos, group);
    this->layout->insertWidget(pos, this->groups.at(pos));

    this->connect(group, &GroupIcons::clicked, this, [=](){
        emit this->changeGroupRequested(pos);
    });
}

void GroupSelectorDialog::open(){
    QWidget *parent = parentWidget();
    if (parent) {
        QRect parentGeom = parent->geometry();
        int x = parentGeom.x() + (parentGeom.width() - width()) / 2;
        int y = parentGeom.y() + (parentGeom.height() - height()) - 20;
        move(x, y);
    }
    QDialog::open();
}

GroupSelectorDialog::~GroupSelectorDialog(){
    for(GroupIcons *group: this->groups){
        delete group;
    }
    this->groups.clear();
    delete this->layout;
}
