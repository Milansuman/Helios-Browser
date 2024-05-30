#pragma once

#include <QLabel>
#include <QMouseEvent>
#include "../browserwindow.h"

class AddressBox : public QLabel {
    Q_OBJECT
private:
    BrowserWindow *window;
public:
    AddressBox(QString text, BrowserWindow *window, QWidget *parent = nullptr);
    ~AddressBox();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};