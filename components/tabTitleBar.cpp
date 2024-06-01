#include "tabTitleBar.h"
#include "searchDialog.h"

TabTitleBar::TabTitleBar(bool isMainWindow, QWidget *parent): QWidget(parent){
    this->setFixedHeight(25);
    this->setContentsMargins(0, 0, 0, 0);
    this->titlebarLayout = new QHBoxLayout(this);

    this->searchDialog = new SearchDialog(this);

    this->addressBox = new AddressBox(isMainWindow ? "search or enter link." : "");
    this->addressBox->setBlank(isMainWindow);

    this->titlebarLayout->addStretch();
    this->titlebarLayout->addWidget(this->addressBox);
    this->titlebarLayout->addStretch();

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
}