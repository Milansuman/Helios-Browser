#include "BookMarkGroup.h"

BookMarkGroup::BookMarkGroup(QString title, QList<struct Bookmark> bookmarks, QWidget *parent): QWidget(parent){
    this->layout = new QVBoxLayout(this);
    this->layout->setSpacing(10);

    this->title = new QLabel(title);
    this->title->setStyleSheet(R"(
        QLabel{
            color: rgb(135, 135, 135);
        }
    )");

    this->layout->addWidget(this->title);

    for(struct Bookmark bookmark: bookmarks){
        QPushButton *button = new QPushButton(bookmark.icon, bookmark.title);
        button->setStyleSheet(R"(
            QPushButton{
                background-color: transparent;
                border: none;
                text-align: left;
                padding: 5px;
                border-radius: 5px;
            }

            QPushButton:hover{
                background-color: rgba(0, 0, 0, 0.5);
            }
        )");

        this->connect(button, &QPushButton::clicked, this, [=](){
            emit this->loadRequested(bookmark.url);
        });

        this->bookmarks.insert(button, bookmark);

        this->layout->addWidget(button);
    }
}

BookMarkGroup::~BookMarkGroup() = default;