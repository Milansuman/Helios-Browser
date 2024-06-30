#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <vector>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QIcon>
#include <QMenu>

#include "TabGroup.h"

class GroupIcons: public QWidget{
    Q_OBJECT
private:
    std::vector<QLabel*> favicons;
    QHBoxLayout *layout;
    QMenu *menu;
public:
    GroupIcons(QWidget *parent=nullptr);
    GroupIcons(TabGroup *group, QWidget *parent=nullptr);
    void addFavicon(QIcon favicon);
    void insertFavicon(int pos, QIcon favicon);
    void deleteFavicon(int pos);
    ~GroupIcons();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void clicked();
    void closeGroupRequested();
};