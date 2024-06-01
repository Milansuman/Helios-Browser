#include "searchDialog.h"
#include <QDialog>
#include <QHBoxLayout>
#include <QPixmap>
#include <QLineEdit>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QKeyEvent>

SearchDialog::SearchDialog(QWidget *parent): QDialog(parent), searchText(""){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedWidth(500);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 237));
    setPalette(palette);

    searchLayout = new QHBoxLayout();
    searchLayout->setContentsMargins(10, 10, 10, 10);

    //Search Icon
    searchIconLabel = new QLabel();
    QPixmap searchIcon(":/icons/search.png");
    searchIconLabel->setPixmap(searchIcon);
    searchIconLabel->setScaledContents(true);
    searchIconLabel->setFixedSize(20, 20);

    //Search input
    searchbar = new QLineEdit();
    searchbar->setPlaceholderText("Search or Enter URL");
    searchbar->setStyleSheet(
        "QLineEdit{"
        "   background: transparent;"
        "   border: none;"
        "}"
    );
    
    searchLayout->addWidget(searchIconLabel);
    searchLayout->addWidget(searchbar);
    this->setLayout(searchLayout);
}

void SearchDialog::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10); // Adjust the radius for rounded corners

    // Draw the background
    painter.fillPath(path, palette().window());

    // Draw the border
    painter.setPen(QPen(QColor(92, 92, 92), 1));
    painter.drawPath(path);

    // Call the base class implementation
    QDialog::paintEvent(event);
}

void SearchDialog::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
        this->searchText = this->searchbar->text();
        this->accept();
        this->searchText = "";
        this->searchbar->setText("");
    }
}

QString SearchDialog::getSearch(){
    return this->searchText;
}

SearchDialog::~SearchDialog(){
    delete this->searchIconLabel;
    delete this->searchLayout;
    delete this->searchbar;
}