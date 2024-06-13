#pragma once

#include <QPushButton>
#include <QPixmap>

class IconButton : public QPushButton {
    Q_OBJECT
private:
    QPixmap *icon;
public:
    IconButton(QString filename, QWidget *parent=nullptr);
    void scale(int w, int h);
    void setButtonIcon(QString filename);
    ~IconButton();
};