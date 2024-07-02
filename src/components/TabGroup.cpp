#include "TabGroup.h"

TabGroup::TabGroup(QWebEngineProfile *profile, QWidget *parent): QSplitter(parent), profile(profile){
    this->setMouseTracking(true);
    this->setStyleSheet(
        "QSplitter::handle{"
        "   background: transparent;"
        "}"
    );
    this->setHandleWidth(5);
    this->tabs.push_back(new Tab(profile));
    this->tabs.at(0)->setTitleBarVisible(false);
    this->activeTab = this->tabs.at(0);

    this->addWidget(this->tabs.at(0));

    this->connect(this->tabs.at(0), &Tab::titleChanged, this, [=](QString title){
        emit this->windowTitleChanged(title);
    });

    this->connect(this->tabs.at(0), &Tab::iconChanged, this, [=](){
        emit this->tabIconChanged();
    });

    this->connect(this->tabs.at(0), &Tab::tabFocused, this, [=](){
        this->activeTab = this->tabs.at(0);
    });

    this->connect(this->tabs.at(0), &Tab::newTabRequested, this, [=](QUrl url){
        emit this->newTabRequested(url);
    });

    this->connect(this->tabs.at(0), &Tab::newWindowRequested, this, [=](QUrl url){
        emit this->newWindowRequested(url);
    });

    this->connect(this, &TabGroup::tabsChanged, this, [=](){
        for(Tab* tab: this->tabs){
            tab->setTitleBarVisible(this->tabs.size() > 1);
        }

        if(this->tabs.size() == 1){
            emit this->windowTitleChanged(this->tabs.at(0)->getTitle());
        }
    });
    
    Tab *temp = this->tabs.at(0);
    connect(this->tabs.at(0), &Tab::splitTabLeftRequested, this, [=](){
        this->splitLeft(this->findTab(temp)); //the position of tabs may change, we use the Tab pointer to find the new position
    });

    connect(this->tabs.at(0), &Tab::splitTabRightRequested, this, [=](){
        this->splitRight(this->findTab(temp));
    });

    connect(this->tabs.at(0), &Tab::splitTabFlipRequested, this, [=](){
        if(this->orientation() == Qt::Orientation::Horizontal){
            this->setOrientation(Qt::Orientation::Vertical);
        }else{
            this->setOrientation(Qt::Orientation::Horizontal);
        }
    });

    connect(this->tabs.at(0), &Tab::closeTabRequested, this, [=](){
        this->removeTab(this->findTab(temp));
    });
}

int TabGroup::findTab(Tab *tab){
    std::vector<Tab*>::iterator tabIter = std::find(this->tabs.begin(), this->tabs.end(), tab);
    return std::distance(this->tabs.begin(), tabIter);
}

void TabGroup::splitLeft(int pos){
    if(this->tabs.size() == 4){
        return;
    }

    this->tabs.insert(this->tabs.begin()+pos, new Tab(this->profile));
    this->insertWidget(pos, this->tabs.at(pos));

    Tab *temp = this->tabs.at(pos);

    connect(this->tabs.at(pos), &Tab::iconChanged, this, [=](){
        emit this->tabIconChanged();
    });

    connect(this->tabs.at(pos), &Tab::splitTabLeftRequested, this, [=](){
        this->splitLeft(this->findTab(temp)); //the position of tabs may change, we use the Tab pointer to find the new position
    });

    connect(this->tabs.at(pos), &Tab::splitTabRightRequested, this, [=](){
        this->splitRight(this->findTab(temp));
    });

    connect(this->tabs.at(pos), &Tab::splitTabFlipRequested, this, [=](){
        if(this->orientation() == Qt::Orientation::Horizontal){
            this->setOrientation(Qt::Orientation::Vertical);
        }else{
            this->setOrientation(Qt::Orientation::Horizontal);
        }
    });

    connect(this->tabs.at(pos), &Tab::closeTabRequested, this, [=](){
        this->removeTab(this->findTab(temp));
    });

    connect(this->tabs.at(pos), &Tab::tabFocused, this, [=](){
        this->activeTab = temp;
    });

    connect(this->tabs.at(pos), &Tab::newTabRequested, this, [=](QUrl url){
        emit this->newTabRequested(url);
    });

    connect(this->tabs.at(pos), &Tab::newWindowRequested, this, [=](QUrl url){
        emit this->newWindowRequested(url);
    });

    emit this->tabIconChanged();
    emit this->tabsChanged();
}

void TabGroup::splitRight(int pos){
    if(this->tabs.size() == 4){
        return;
    }
    this->tabs.insert(this->tabs.begin()+pos+1, new Tab(this->profile));
    this->insertWidget(pos+1, this->tabs.at(pos+1));

    Tab *temp = this->tabs.at(pos+1);

    connect(this->tabs.at(pos+1), &Tab::iconChanged, this, [=](){
        emit this->tabIconChanged();
    });

    connect(this->tabs.at(pos+1), &Tab::splitTabLeftRequested, this, [=](){
        this->splitLeft(this->findTab(temp)); //the position of tabs may change, we use the Tab pointer to find the new position
    });

    connect(this->tabs.at(pos+1), &Tab::splitTabRightRequested, this, [=](){
        this->splitRight(this->findTab(temp));
    });

    connect(this->tabs.at(pos+1), &Tab::splitTabFlipRequested, this, [=](){
        if(this->orientation() == Qt::Orientation::Horizontal){
            this->setOrientation(Qt::Orientation::Vertical);
        }else{
            this->setOrientation(Qt::Orientation::Horizontal);
        }
    });

    connect(this->tabs.at(pos+1), &Tab::closeTabRequested, this, [=](){
        this->removeTab(this->findTab(temp));
    });

    connect(this->tabs.at(pos+1), &Tab::tabFocused, this, [=](){
        this->activeTab = temp;
    });

    connect(this->tabs.at(pos+1), &Tab::newTabRequested, this, [=](QUrl url){
        emit this->newTabRequested(url);
    });

    connect(this->tabs.at(pos+1), &Tab::newWindowRequested, this, [=](QUrl url){
        emit this->newWindowRequested(url);
    });

    emit this->tabIconChanged();
    emit this->tabsChanged();
}

void TabGroup::removeTab(int pos){
    this->tabs.at(pos)->hide();
    delete this->tabs.at(pos);
    this->tabs.erase(this->tabs.begin()+pos);

    emit this->tabIconChanged();
    emit this->tabsChanged();
}

std::vector<Tab*> TabGroup::getTabs(){
    return this->tabs;
}

Tab* TabGroup::getTab(int pos){
    return this->tabs.at(pos);
}

void TabGroup::openDevTools(){
    this->activeTab->openDevTools();
}

TabGroup::~TabGroup(){
    for(Tab *tab: this->tabs){
        delete tab;
    }
    this->tabs.clear();
}