#include "GroupIcons.h"
#include "Tab.h"

#include <QPainter>
#include <QAction>

GroupIcons::GroupIcons(QWidget *parent): QWidget(parent){
    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(10, 10, 10, 10);
    this->layout->setSpacing(10);
}

GroupIcons::GroupIcons(TabGroup *group, QWidget *parent): QWidget(parent){
    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(10, 10, 10, 10);
    this->layout->setSpacing(10);

    this->menu = new QMenu(this);
    this->menu->setStyleSheet(
        "QMenu{"
        "   background: rgb(30,30,30);"
        "   border-radius: 10px;"
        "   border: none;"
        "}"
    );

    this->menu->addAction(QIcon(":/icons/white/tab-close.png"), "Close Group");

    this->connect(this->menu->actions().at(0), &QAction::triggered, this, [=](){
        emit this->closeGroupRequested();
    });

    for(Tab *tab: group->getTabs()){
        this->addFavicon(!tab->getIcon().isNull() ? tab->getIcon() : QIcon(":/icons/earth.png"));
    }

    this->connect(group, &TabGroup::tabIconChanged, this, [=](){
        for(QLabel *favicon: this->favicons){
            delete favicon;
        }
        this->favicons.clear();

        for(Tab *tab: group->getTabs()){
            this->addFavicon(!tab->getIcon().isNull() ? tab->getIcon() : QIcon(":/icons/earth.png"));
        }
        this->layout->update();
        this->updateGeometry(); //force geometry update to fix issue with layout cramping
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

void GroupIcons::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit this->clicked();
    }else if(event->button() == Qt::RightButton){
        int height = this->menu->sizeHint().height();
        this->menu->popup(this->mapToGlobal(QPoint(0,-10-height)));
    }
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