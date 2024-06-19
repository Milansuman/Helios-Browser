#include "AuthenticationDialog.h"

#include <QFontDatabase>
#include <QFont>

#include <QPainter>

AuthenticationDialog::AuthenticationDialog(QWidget *parent): QDialog(parent), authenticator(nullptr){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont *font = new QFont(fontFamily, -1, QFont::Bold);
    QFont *heading = new QFont(fontFamily, 12, QFont::Bold);

    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(30, 30, 30, 30);

    this->infoLabel = new QLabel();
    this->infoLabel->setFont(*heading);
    qDebug() << "Info label font:" << this->infoLabel->font();
    this->usernameLabel = new QLabel("Username");
    this->passwordLabel = new QLabel("Password");

    this->username = new QLineEdit();
    this->username->setFixedHeight(30);
    this->username->setStyleSheet(
        "QLineEdit{"
        "   border: none;"
        "   background-color: rgb(62, 62, 62);"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "}"
    );

    this->password = new QLineEdit();
    this->password->setFixedHeight(30);
    this->password->setStyleSheet(
        "QLineEdit{"
        "   border: none;"
        "   background-color: rgb(62, 62, 62);"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "}"
    );

    this->submit = new QPushButton("Sign In");
    this->submit->setFont(*font);
    this->submit->setStyleSheet(
        "QPushButton{"
        "   border: none;"
        "   background-color: rgb(1, 71, 255);"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "}"
    );

    this->cancel = new QPushButton("Cancel");
    this->cancel->setFont(*font);
    this->cancel->setStyleSheet(
        "QPushButton{"
        "   border: none;"
        "   background-color: rgb(255, 67, 67);"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "}"
    );

    this->connect(this->submit, &QPushButton::clicked, this, [=](){
        this->authenticator->setUser(this->username->text());
        this->authenticator->setPassword(this->password->text());
        this->accept();
    });

    this->connect(this->cancel, &QPushButton::clicked, this, [=](){
        this->reject();
    });

    this->layout->addWidget(this->infoLabel);
    this->layout->addWidget(this->usernameLabel);
    this->layout->addWidget(this->username);
    this->layout->addWidget(this->passwordLabel);
    this->layout->addWidget(this->password);
    this->layout->addWidget(this->submit);
    this->layout->addWidget(this->cancel);
}

void AuthenticationDialog::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(30, 30, 30)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
}

void AuthenticationDialog::exec(const QUrl &requestUrl, QAuthenticator *authenticator){
    this->infoLabel->setText(requestUrl.toString());
    this->authenticator = authenticator;
    QDialog::exec();
}

AuthenticationDialog::~AuthenticationDialog() = default;