#include "GroupIcons.h"
#include "Tab.h"

#include <QPainter>

GroupIcons::GroupIcons(QWidget *parent): QWidget(parent){
    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(10, 10, 10, 10);
    this->layout->setSpacing(10);
}

GroupIcons::GroupIcons(TabGroup *group, QWidget *parent): QWidget(parent){
    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(10, 10, 10, 10);
    this->layout->setSpacing(10);

    for(Tab *tab: group->getTabs()){
        this->addFavicon(tab->getIcon());
    }

    this->connect(group, &TabGroup::tabIconChanged, this, [=](){
        for(QLabel *favicon: this->favicons){
            delete favicon;
        }
        this->favicons.clear();

        for(Tab *tab: group->getTabs()){
            this->addFavicon(tab->getIcon());
        }
        this->layout->update();
    });
}

void GroupIcons::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(50, 50, 50)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 12, 12);
    QWidget::paintEvent(event);
}

void GroupIcons::addFavicon(QIcon favicon){
    this->favicons.push_back(new QLabel());
    this->favicons.at(this->favicons.size()-1)->setFixedSize(30, 30);
    this->favicons.at(this->favicons.size()-1)->setPixmap(favicon.pixmap(30, 30));

    this->layout->addWidget(this->favicons.at(this->favicons.size()-1));
    this->updateGeometry(); //force geometry update to fix issue with layout cramping
}

void GroupIcons::insertFavicon(int pos, QIcon favicon){
    this->favicons.insert(this->favicons.begin()+pos, new QLabel());
    this->favicons.at(pos)->setFixedSize(30, 30);
    this->favicons.at(pos)->setPixmap(favicon.pixmap(30, 30));

    this->layout->insertWidget(pos, this->favicons.at(pos));
    this->updateGeometry(); //force geometry update to fix issue with layout cramping
}

void GroupIcons::deleteFavicon(int pos){
    delete this->favicons.at(pos);
    this->updateGeometry(); //force geometry update to fix issue with layout cramping
}

GroupIcons::~GroupIcons(){
    for(QLabel *favicon: this->favicons){
        delete favicon;
    }
    this->favicons.clear();
    delete this->layout;
}