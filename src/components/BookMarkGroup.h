#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QList>
#include <QMap>
#include <QPixmap>
#include <QPushButton>

struct Bookmark{
    QPixmap icon;
    QString title;
    QUrl url;
};

class BookMarkGroup: public QWidget{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    QLabel *title;
    QMap<QPushButton*, struct Bookmark> bookmarks;
public:
    BookMarkGroup(QString title, QList<struct Bookmark> bookmarks, QWidget *parent=nullptr);
    ~BookMarkGroup();
signals:
    void loadRequested(QUrl url);
};