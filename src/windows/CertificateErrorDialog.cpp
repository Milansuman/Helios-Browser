#include "CertificateErrorDialog.h"
#include <QFontDatabase>
#include <QFont>
#include <QPainter>

CertificateErrorDialog::CertificateErrorDialog(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMaximumWidth(400);  // Increased width for more text space

    QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont *font = new QFont(fontFamily, -1, QFont::Bold);

    this->layout = new QVBoxLayout(this);

    this->title = new QLabel("Certificate Error");
    this->title->setFont(*font);
    this->title->setStyleSheet("color: rgb(200, 200, 200);");

    this->description = new QLabel("");
    this->description->setFont(*font);
    this->description->setStyleSheet("color: rgb(150, 150, 150);");
    this->description->setWordWrap(true);

    this->ok = new QPushButton("Continue");
    this->ok->setFont(*font);
    this->ok->setStyleSheet(
        "QPushButton{"
        "    border: none;"
        "    background-color: rgb(200, 200, 200);"
        "    color: rgb(30, 30, 30);"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "}"
    );

    this->cancel = new QPushButton("Go Back");
    this->cancel->setFont(*font);
    this->cancel->setStyleSheet(
        "QPushButton{"
        "    border: none;"
        "    background-color: rgb(60, 60, 60);"
        "    color: rgb(200, 200, 200);"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "}"
    );

    this->layout->addWidget(this->title);
    this->layout->addWidget(this->description);
    this->layout->addWidget(this->ok);
    this->layout->addWidget(this->cancel);

    connect(this->ok, &QPushButton::clicked, this, &QDialog::accept);
    connect(this->cancel, &QPushButton::clicked, this, &QDialog::reject);
}

void CertificateErrorDialog::exec(QWebEngineCertificateError certificateError)
{
    this->description->setText(certificateError.description());
    QDialog::exec();
}

void CertificateErrorDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(QColor(30, 30, 30)));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 10, 10);
    QDialog::paintEvent(event);
}

CertificateErrorDialog::~CertificateErrorDialog() = default;