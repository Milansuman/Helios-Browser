#pragma once

#include <QLabel>
#include <QMouseEvent>
#include "../browserwindow.h"

class AddressBox : public QLabel {
    Q_OBJECT
private:
    bool isBlank;
public:
    AddressBox(QString text, QWidget *parent = nullptr);
    void setBlank(bool blank);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void requestedSearchDialog();
};