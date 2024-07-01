#pragma once

#include <QWidget>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>

class PreviewWindow : public QWidget{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    QWidget *titlebar;
    QHBoxLayout *titleBarLayout;
    QLabel *title, *preview;
public:
    PreviewWindow(QString title, QPixmap *preview=nullptr, QWidget *parent=nullptr);
    ~PreviewWindow();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
signals:
    void clicked();
};