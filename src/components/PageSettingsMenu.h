#pragma once

#include <QWidgetAction>
#include "IconButton.h"

class PermissionsAction : public QWidgetAction{
    Q_OBJECT
public:
    PermissionsAction(QObject *parent=nullptr);
    ~PermissionsAction();
};

class PageSettingsMenu : public IconButton{
    Q_OBJECT
public:
    PageSettingsMenu(QWidget *parent=nullptr);
    ~PageSettingsMenu();
};