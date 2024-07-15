#pragma once

#include <QWidget>
#include <QPaintEvent>

class Separator: public QWidget{
    Q_OBJECT
public:
    Separator(QWidget *parent=nullptr);
    ~Separator();
protected:
    void paintEvent(QPaintEvent *event) override;
};