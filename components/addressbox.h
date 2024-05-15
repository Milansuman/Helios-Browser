#pragma once

#include <QLabel>

class AddressBox : public QLabel {
    Q_OBJECT
public:
    AddressBox(QString text, QWidget *parent = nullptr);
    ~AddressBox();
protected:
    void paintEvent(QPaintEvent *event) override;
};