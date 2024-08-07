#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

#include "IconButton.h"
#include "BookMarkGroup.h"

class SideBar : public QWidget{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    QHBoxLayout *bottomRowLayout;
    BookMarkGroup *programmingBookMarkGroup, *designBookMarkGroup, *socialBookMarkGroup;
    IconButton *newGroupButton, *gridViewButton;
public:
    SideBar(QWidget *parent=nullptr);
    ~SideBar();
signals:
    void newGroupRequested();
    void openURL(QUrl url);
};