#pragma once

#include <QMenu>
#include <QWidget>
#include <QHBoxLayout>

#include "addressbox.h"
#include "searchDialog.h"
#include "splitTabMenu.h"
#include "iconButton.h"

class TabTitleBar : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout* titlebarLayout;
    AddressBox* addressBox;
    SearchDialog *searchDialog;
    SplitTabMenu *splitTabMenuAction;
    QMenu *splitTabMenu;
public:
    TabTitleBar(bool isMainWindow=false, QWidget *parent=nullptr);
    void setTitle(QString title);
    ~TabTitleBar();
signals:
    void search(QString search);
    void splitLeftRequested();
    void splitRightRequested();
};