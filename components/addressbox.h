#pragma once

#include <QLabel>
#include <QMouseEvent>
#include "../browserwindow.h"

class AddressBox : public QLabel {
    Q_OBJECT
private:
    BrowserWindow *window;
    bool isBlank;
public:
    AddressBox(QString text, BrowserWindow *window, QWidget *parent = nullptr);
    void setBlank(bool blank);
    ~AddressBox();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};