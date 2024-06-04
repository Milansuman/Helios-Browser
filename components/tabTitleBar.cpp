#include "tabTitleBar.h"
#include "searchDialog.h"
#include "splitTabMenu.h"

TabTitleBar::TabTitleBar(bool isMainWindow, QWidget *parent): QWidget(parent){
    this->setFixedHeight(30);
    this->setContentsMargins(0, 0, 0, 5);
    this->titlebarLayout = new QHBoxLayout(this);

    this->searchDialog = new SearchDialog(this);

    this->splitTabMenu = new QMenu(this);
    this->splitTabMenu->setIcon(QIcon(":/icons/split.png"));

    this->splitTabMenuAction = new SplitTabMenu();
    this->splitTabMenu->addAction(this->splitTabMenuAction);    
    //this->splitTabMenu->popup(QWidget::mapToGlobal(this->pos()));

    this->addressBox = new AddressBox(isMainWindow ? "search or enter link." : "");
    this->addressBox->setBlank(isMainWindow);

    this->titlebarLayout->addStretch();
    this->titlebarLayout->addWidget(this->addressBox);
    this->titlebarLayout->addStretch();
    this->titlebarLayout->addWidget(this->splitTabMenu);

    connect(this->addressBox, &AddressBox::requestedSearchDialog, this, [=](){
        this->searchDialog->open();
    });

    connect(this->searchDialog, &SearchDialog::accepted, this, [=](){
        emit this->search(this->searchDialog->getSearch());
    });
}

void TabTitleBar::setTitle(QString title){
    this->addressBox->setText(title);
}

TabTitleBar::~TabTitleBar(){
    delete this->titlebarLayout;
    delete this->addressBox;
    delete this->searchDialog;
    delete this->splitTabMenu;
    delete this->splitTabMenuAction;
}