#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include "addressbox.h"
#include "searchDialog.h"

class TabTitleBar : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout* titlebarLayout;
    AddressBox* addressBox;
    SearchDialog *searchDialog;
public:
    TabTitleBar(bool isMainWindow=false, QWidget *parent=nullptr);
    void setTitle(QString title);
    ~TabTitleBar();
signals:
    void search(QString search);
};