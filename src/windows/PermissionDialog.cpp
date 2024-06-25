#include "PermissionDialog.h"


#include <QFontDatabase>
#include <QFont>
#include <QPainter>

PermissionDialog::PermissionDialog(QWidget *parent): QDialog(parent){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont *font = new QFont(fontFamily, -1, QFont::Bold);

    this->dialogLayout = new QVBoxLayout(this);

    this->messageLabel = new QLabel("");
    this->messageLabel->setFont(*font);

    this->ok = new QPushButton("Allow");
    this->ok->setFont(*font);
    this->ok->setStyleSheet(
        "QPushButton{"
        "   border: none;"
        "   background-color: rgb(1, 71, 255);"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "}"
    );

    this->cancel = new QPushButton("Reject");
    this->cancel->setFont(*font);
    this->cancel->setStyleSheet(
        "QPushButton{"
        "   border: none;"
        "   background-color: rgb(255, 67, 67);"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "}"
    );

    this->dialogLayout->addWidget(this->messageLabel);
    this->dialogLayout->addWidget(this->ok);
    this->dialogLayout->addWidget(this->cancel);

    this->connect(this->ok, &QPushButton::clicked, this, [=](){
        this->accept();
    });

    this->connect(this->cancel, &QPushButton::clicked, this, [=](){
        this->reject();
    });
}

void PermissionDialog::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(30, 30, 30)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
    QDialog::paintEvent(event);
}

void PermissionDialog::exec(QString message){
    this->messageLabel->setText(message);
    QDialog::exec();
}

PermissionDialog::~PermissionDialog() = default;