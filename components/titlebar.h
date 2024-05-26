#pragma once

#include <QWidget>
#include <QMouseEvent>
#include "../browserwindow.h"

class TitleBar : public QWidget {
    Q_OBJECT
public:
    TitleBar(BrowserWindow *window, QWidget *parent = nullptr);
    ~TitleBar();
};