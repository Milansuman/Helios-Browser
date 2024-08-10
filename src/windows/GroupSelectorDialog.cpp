#include "GroupSelectorDialog.h"
#include <QPropertyAnimation>
#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

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
        emit this->changeGroupRequested(this->findGroup(group));
    });

    this->connect(group, &GroupIcons::closeGroupRequested, this, [=](){
        emit this->closeGroupRequested(pos);
        delete group;
        this->groups.erase(this->groups.begin()+pos);
    });
}

void GroupSelectorDialog::insertGroup(int pos, GroupIcons *group){
    this->groups.insert(this->groups.begin()+pos, group);
    this->layout->insertWidget(pos, this->groups.at(pos));

    this->connect(group, &GroupIcons::clicked, this, [=](){
        emit this->changeGroupRequested(this->findGroup(group));
    });

    this->connect(group, &GroupIcons::closeGroupRequested, this, [=](){
        emit this->closeGroupRequested(pos);
        delete group;
        this->groups.erase(this->groups.begin()+pos);
    });
}

void GroupSelectorDialog::open() {
    if (this->parentWidget()) {
        this->setFixedSize(this->sizeHint());
        QPoint newPos = this->parentWidget()->mapToGlobal(this->parentWidget()->rect().center() + QPoint(-this->width()/2, this->parentWidget()->rect().height()/2 - this->height() - 20));
        move(newPos);

        QDialog::open();

        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        setGraphicsEffect(effect);

        QPropertyAnimation *opacityAnimation = new QPropertyAnimation(effect, "opacity");
        opacityAnimation->setDuration(200);
        opacityAnimation->setStartValue(0.0);
        opacityAnimation->setEndValue(1.0);
        opacityAnimation->setEasingCurve(QEasingCurve::OutQuad);

        QSequentialAnimationGroup *heightAnimationGroup = new QSequentialAnimationGroup(this);

        QPropertyAnimation *growAnimation = new QPropertyAnimation(this, "geometry");
        growAnimation->setDuration(400);
        growAnimation->setStartValue(QRect(newPos.x(), newPos.y() + height(), width(), 0)); // Start with a height of 0 (collapsed)
        growAnimation->setEndValue(QRect(newPos.x(), newPos.y() - 10, width(), height() + 20)); // Overshoot the target position
        growAnimation->setEasingCurve(QEasingCurve::OutCubic);

        QPropertyAnimation *returnAnimation = new QPropertyAnimation(this, "geometry");
        returnAnimation->setDuration(200);
        returnAnimation->setStartValue(QRect(newPos.x(), newPos.y() - 10, width(), height() + 20)); // Start from the overshoot position
        returnAnimation->setEndValue(QRect(newPos.x(), newPos.y(), width(), height())); // End at the correct position
        returnAnimation->setEasingCurve(QEasingCurve::OutCubic);

        heightAnimationGroup->addAnimation(growAnimation);
        heightAnimationGroup->addAnimation(returnAnimation);

        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
        group->addAnimation(opacityAnimation);
        group->addAnimation(heightAnimationGroup);
        group->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        QDialog::open();
    }
}

int GroupSelectorDialog::findGroup(GroupIcons *group){
    std::vector<GroupIcons*>::iterator it = std::find(this->groups.begin(), this->groups.end(), group);
    return std::distance(this->groups.begin(), it);
}

GroupSelectorDialog::~GroupSelectorDialog(){
    for(GroupIcons *group: this->groups){
        delete group;
    }
    this->groups.clear();
    delete this->layout;
}
