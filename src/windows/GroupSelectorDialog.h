#pragma once

#include <QDialog>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <vector>

#include "../components/GroupIcons.h"

class GroupSelectorDialog : public QDialog{
    Q_OBJECT
private:
    std::vector<GroupIcons*> groups;
    QHBoxLayout *layout;

    int findGroup(GroupIcons *group);
public:
    GroupSelectorDialog(QWidget *parent=nullptr);
    void addGroup(GroupIcons *group);
    void insertGroup(int pos, GroupIcons *group);
    void open();
    ~GroupSelectorDialog();
protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void changeGroupRequested(int pos);
    void closeGroupRequested(int pos);
};