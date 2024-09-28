#pragma once

#include <QQuickWidget>
#include <QQuickItem>
#include <QUrl>

class ExtensionWrapper: public QQuickWidget{
    Q_OBJECT
private:
    QQuickItem *root;
    QString name, description, version;
    void initialize(QUrl path);
public:
    ExtensionWrapper(QUrl path, QWidget *parent=nullptr);
    ~ExtensionWrapper();
protected:
    //void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
};