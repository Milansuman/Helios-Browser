#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <vector>
#include <QPaintEvent>
#include <QIcon>

#include "TabGroup.h"

class GroupIcons: public QWidget{
    Q_OBJECT
private:
    std::vector<QLabel*> favicons;
    QHBoxLayout *layout;
public:
    GroupIcons(QWidget *parent=nullptr);
    GroupIcons(TabGroup *group, QWidget *parent=nullptr);
    void addFavicon(QIcon favicon);
    void insertFavicon(int pos, QIcon favicon);
    void deleteFavicon(int pos);
    ~GroupIcons();
protected:
    void paintEvent(QPaintEvent *event) override;
};