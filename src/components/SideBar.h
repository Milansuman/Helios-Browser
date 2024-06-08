#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

#include "IconButton.h"

class SideBar : public QWidget{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    QHBoxLayout *bottomRowLayout;
    IconButton *newGroupButton, *gridViewButton;
public:
    SideBar(QWidget *parent=nullptr);
    ~SideBar();
};