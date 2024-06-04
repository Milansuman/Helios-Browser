#pragma once

#include <QDialog>
#include <QHBoxLayout>
#include <QPaintEvent>
#include "iconButton.h"

class SplitTabDialog : public QDialog{
    Q_OBJECT
private:
    IconButton *splitLeftButton;
    IconButton *splitRightButton;
    QHBoxLayout *layout;
public:
    SplitTabDialog(QWidget *parent=nullptr);
    void open();
    ~SplitTabDialog();
protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void splitLeftRequested();
    void splitRightRequested();
};