#pragma once

#include <QWidgetAction>
#include <QHBoxLayout>
#include <QWidget>

#include "iconButton.h"

class SplitTabMenu : public QWidgetAction {
    Q_OBJECT
private:
    QWidget *actionWidget;
    QHBoxLayout *layout;
    IconButton *splitLeftButton;
    IconButton *splitRightButton;
public:
    SplitTabMenu(QObject *parent=nullptr);
    ~SplitTabMenu();
};