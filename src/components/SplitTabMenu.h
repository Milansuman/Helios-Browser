#pragma once

#include <QWidget>
#include <QWidgetAction>
#include <QMenu>
#include <QHBoxLayout>

#include "IconButton.h"

class SplitTabMenuAction : public QWidgetAction {
    Q_OBJECT
private:
    QWidget *defaultWidget;
    QHBoxLayout *layout;
    IconButton *splitTabLeftButton, *splitTabRightButton, *splitTabFlipButton;
public:
    SplitTabMenuAction(QObject *parent=nullptr);
    ~SplitTabMenuAction();
signals:
    void splitTabLeftRequested();
    void splitTabRightRequested();
    void splitTabFlipRequested();
};

class SplitTabMenu : public IconButton{
    Q_OBJECT
private:
    QMenu *menu;
    SplitTabMenuAction *splitAction;
public:
    SplitTabMenu(QWidget *parent=nullptr);
    ~SplitTabMenu();
signals:
    void splitTabLeftRequested();
    void splitTabRightRequested();
    void splitTabFlipRequested();
};