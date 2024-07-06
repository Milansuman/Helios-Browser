#pragma once

#include <QWidgetAction>
#include <QMenu>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include "./IconButton.h"

class SoundButton: public QPushButton{
    Q_OBJECT
private:
    bool isSoundOn;
public:
    SoundButton(QWidget *parent=nullptr);
    ~SoundButton();
};

class PermissionsAction: public QWidgetAction{
    Q_OBJECT
private:
    QWidget *defaultWidget;
    QVBoxLayout *layout;
    QHBoxLayout *titleLayout;
    QGridLayout *permissionsLayout;
    QLabel *urlLabel;
    SoundButton *soundButton;
public:
    PermissionsAction(QObject *parent=nullptr);
    void setTitle(QString title);
    ~PermissionsAction();
};

class PageSettings: public IconButton{
    Q_OBJECT
private:
    QMenu *menu;
    PermissionsAction *permissionsAction;
public:
    PageSettings(QWidget *parent=nullptr);
    void setTitle(QString title);
    ~PageSettings();
};