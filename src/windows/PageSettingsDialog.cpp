#include "PageSettingsDialog.h"

#include <QPainter>
#include <QFontDatabase>
#include <QFont>

SoundButton::SoundButton(QWidget *parent): QPushButton(parent){
    int i = QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    QFont font(QFontDatabase::applicationFontFamilies(i).at(0), -1, QFont::Bold);
    this->setText("Sound");
    this->setFont(font);
    this->setIcon(QIcon(":/icons/black/audio.png"));

    this->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(228, 228, 228);"
        "   border-radius: 6px;"
        "   color: rgb(30, 30, 30);"
        "}"
    );

    this->setFixedHeight(40);
}

SoundButton::~SoundButton() = default;

PermissionsGroup::PermissionsGroup(QWidget *parent): QWidget(parent){
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(1, 1, 1, 1);
    this->setStyleSheet(
        "QWidget{"
        "   background: white;"
        "}"
    );

    this->notifications = new QPushButton(QIcon(":/icons/white/bell-dot.png"), "Notifications");
    this->notifications->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(49, 53, 60);"
        "   border-radius: 6px;"
        "}"
    );
    this->notifications->setFixedHeight(40);
    this->notifications->setVisible(false);

    this->geolocation = new QPushButton(QIcon(":/icons/white/map-pin.png"), "Location");
    this->geolocation->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(49, 53, 60);"
        "   border-radius: 6px;"
        "   height: 40px;"
        "}"
    );
    this->geolocation->setFixedHeight(40);
    this->geolocation->setVisible(false);

    this->microphone = new QPushButton(QIcon(":/icons/white/mic.png"), "Microphone");
    this->microphone->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(49, 53, 60);"
        "   border-radius: 6px;"
        "   height: 40px;"
        "}"
    );
    this->microphone->setFixedHeight(40);
    this->microphone->setVisible(false);

    this->camera = new QPushButton(QIcon(":/icons/white/camera.png"), "Camera");
    this->camera->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(49, 53, 60);"
        "   border-radius: 6px;"
        "   height: 40px;"
        "}"
    );
    this->camera->setFixedHeight(40);
    this->camera->setVisible(false);

    this->screenShare = new QPushButton(QIcon(":/icons/white/airplay.png"), "Screen Share");
    this->screenShare->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(49, 53, 60);"
        "   border-radius: 6px;"
        "   height: 40px;"
        "}"
    );
    this->screenShare->setFixedHeight(40);
    this->screenShare->setVisible(false);

    this->layout->addWidget(this->notifications);
    this->layout->addWidget(this->geolocation);
    this->layout->addWidget(this->microphone);
    this->layout->addWidget(this->camera);
    this->layout->addWidget(this->screenShare);
}

PermissionsGroup::~PermissionsGroup() = default;

PageSettingsDialog::PageSettingsDialog(QWidget *parent): QDialog(parent), muted(false){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedWidth(200);

    int i = QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    QFont font(QFontDatabase::applicationFontFamilies(i).at(0), -1, QFont::Bold);

    this->layout = new QVBoxLayout(this);
    this->titleLayout = new QHBoxLayout();
    this->title = new QLabel("Document");
    this->title->setFont(font);
    this->title->setMaximumHeight(40);

    this->titleLayout->addStretch();
    this->titleLayout->addWidget(this->title);
    this->titleLayout->addStretch();

    this->soundButton = new SoundButton();

    this->connect(this->soundButton, &QPushButton::clicked, this, [=](){
        if(muted){
            this->soundButton->setIcon(QIcon(":/icons/black/audio.png"));

            this->soundButton->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(228, 228, 228);"
                "   border-radius: 6px;"
                "   color: rgb(30, 30, 30);"
                "}"
            );
        }else{
            this->soundButton->setIcon(QIcon(":/icons/white/audio.png"));

            this->soundButton->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(30, 30, 30);"
                "   border-radius: 6px;"
                "   color: rgb(228, 228, 228);"
                "}"
            );
        }

        this->muted = !this->muted;
        emit this->toggleMuteAudio(this->muted);
    });

    this->permissions = new PermissionsGroup();

    this->layout->addLayout(this->titleLayout);
    this->layout->addWidget(this->soundButton);
    this->layout->addWidget(this->permissions);
}

void PageSettingsDialog::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(30, 30, 30)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
}

void PageSettingsDialog::setUrl(QUrl url){
    this->title->setText(url.host());
}

void PageSettingsDialog::open(){
    this->move(this->parentWidget()->mapToGlobal(this->parentWidget()->geometry().center()) + QPoint(-(this->width()/2), 30));
    QDialog::open();
}

PageSettingsDialog::~PageSettingsDialog() = default;