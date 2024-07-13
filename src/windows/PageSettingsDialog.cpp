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

    this->setFixedHeight(35);
}

SoundButton::~SoundButton() = default;

MenuButton::MenuButton(QPixmap icon, QString text, QWidget *parent): QWidget(parent){
    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(0, 0, 0, 0);
    this->icon = new QLabel();
    this->icon->setPixmap(icon.scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    this->text = new QLabel(text);
    this->arrow = new QLabel();
    this->arrow->setPixmap(QPixmap(":/icons/white/chevron-right.png").scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this->arrow->setFixedHeight(20);

    this->layout->addWidget(this->icon);
    this->layout->addWidget(this->text);
    this->layout->addStretch();
    this->layout->addWidget(this->arrow);
}

void MenuButton::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit this->clicked();
    }
}

MenuButton::~MenuButton() = default;

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
        "   background-color: rgb(81, 169, 79);"
        "   border-radius: 6px;"
        "}"
    );
    this->notifications->setFixedHeight(35);
    this->notifications->setVisible(false);

    this->geolocation = new QPushButton(QIcon(":/icons/white/map-pin.png"), "Location");
    this->geolocation->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(81, 169, 79);"
        "   border-radius: 6px;"
        "   height: 35px;"
        "}"
    );
    this->geolocation->setFixedHeight(35);
    this->geolocation->setVisible(false);

    this->microphone = new QPushButton(QIcon(":/icons/white/mic.png"), "Microphone");
    this->microphone->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(81, 169, 79);"
        "   border-radius: 6px;"
        "   height: 35px;"
        "}"
    );
    this->microphone->setFixedHeight(35);
    this->microphone->setVisible(false);

    this->camera = new QPushButton(QIcon(":/icons/white/camera.png"), "Camera");
    this->camera->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(81, 169, 79);"
        "   border-radius: 6px;"
        "   height: 35px;"
        "}"
    );
    this->camera->setFixedHeight(35);
    this->camera->setVisible(false);

    this->screenShare = new QPushButton(QIcon(":/icons/white/airplay.png"), "Screen Share");
    this->screenShare->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(81, 169, 79);"
        "   border-radius: 6px;"
        "   height: 35px;"
        "}"
    );
    this->screenShare->setFixedHeight(35);
    this->screenShare->setVisible(false);

    this->permissionsMap = new std::map<QPushButton*, bool>({
        {this->notifications, false},
        {this->geolocation, false},
        {this->microphone, false},
        {this->camera, false},
        {this->screenShare, false}
    });

    this->connect(this->notifications, &QPushButton::clicked, this, [=](){
        if(this->permissionsMap->at(this->notifications)){
            this->notifications->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(52, 52, 52);"
                "   border-radius: 6px;"
                "   height: 35px;"
                "}"
            );
        }else{
            this->notifications->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(81, 169, 79);"
                "   border-radius: 6px;"
                "   height: 35px;"
                "}"
            );
        }
        this->permissionsMap->at(this->notifications) = !this->permissionsMap->at(this->notifications);
        emit this->toggleNotifications(this->permissionsMap->at(this->notifications));
    });

    this->connect(this->geolocation, &QPushButton::clicked, this, [=](){
        if(this->permissionsMap->at(this->geolocation)){
            this->geolocation->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(52, 52, 52);"
                "   border-radius: 6px;"
                "   height: 35px;"
                "}"
            );
        }else{
            this->geolocation->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(81, 169, 79);"
                "   border-radius: 6px;"
                "   height: 35px;"
                "}"
            );
        }
        this->permissionsMap->at(this->geolocation) = !this->permissionsMap->at(this->geolocation);
        emit this->toggleGeolocation(this->permissionsMap->at(this->geolocation));
    });

    this->connect(this->camera, &QPushButton::clicked, this, [=](){
        if(this->permissionsMap->at(this->camera)){
            this->camera->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(52, 52, 52);"
                "   border-radius: 6px;"
                "   height: 35px;"
                "}"
            );
        }else{
            this->camera->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(81, 169, 79);"
                "   border-radius: 6px;"
                "   height: 35px;"
                "}"
            );
        }
        this->permissionsMap->at(this->camera) = !this->permissionsMap->at(this->camera);
        emit this->toggleCamera(this->permissionsMap->at(this->camera));
    });

    this->connect(this->microphone, &QPushButton::clicked, this, [=](){
        if(this->permissionsMap->at(this->microphone)){
            this->microphone->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(52, 52, 52);"
                "   border-radius: 6px;"
                "   height: 35px;"
                "}"
            );
        }else{
            this->microphone->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(81, 169, 79);"
                "   border-radius: 6px;"
                "   height: 35px;"
                "}"
            );
        }
        this->permissionsMap->at(this->microphone) = !this->permissionsMap->at(this->microphone);
        emit this->toggleMicrophone(this->permissionsMap->at(this->microphone));
    });

    this->connect(this->screenShare, &QPushButton::clicked, this, [=](){
        if(this->permissionsMap->at(this->screenShare)){
            this->screenShare->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(52, 52, 52);"
                "   border-radius: 6px;"
                "   height: 35px;"
                "}"
            );
        }else{
            this->screenShare->setStyleSheet(
                "QPushButton{"
                "   background-color: rgb(81, 169, 79);"
                "   border-radius: 6px;"
                "   height: 35px;"
                "}"
            );
        }
        this->permissionsMap->at(this->screenShare) = !this->permissionsMap->at(this->screenShare);
        emit this->toggleScreenShare(this->permissionsMap->at(this->screenShare));
    });

    this->layout->addWidget(this->notifications);
    this->layout->addWidget(this->geolocation);
    this->layout->addWidget(this->microphone);
    this->layout->addWidget(this->camera);
    this->layout->addWidget(this->screenShare);
}

void PermissionsGroup::setPermissions(std::map<QWebEnginePage::Feature, bool> permissions){
    if(permissions.at(QWebEnginePage::Feature::Notifications)){
        this->notifications->setVisible(true);
        this->notifications->setStyleSheet(
            "QPushButton{"
            "   background-color: rgb(81, 169, 79);"
            "   border-radius: 6px;"
            "   height: 35px;"
            "}"
        );
        this->permissionsMap->at(this->notifications) = true;
    }else{
        this->notifications->setVisible(false);
        this->notifications->setStyleSheet(
            "QPushButton{"
            "   background-color: rgb(52, 169, 79);"
            "   border-radius: 6px;"
            "   height: 35px;"
            "}"
        );
        this->permissionsMap->at(this->notifications) = false;
    }

    if(permissions.at(QWebEnginePage::Feature::Geolocation)){
        this->geolocation->setVisible(true);
        this->geolocation->setStyleSheet(
            "QPushButton{"
            "   background-color: rgb(81, 169, 79);"
            "   border-radius: 6px;"
            "   height: 35px;"
            "}"
        );
        this->permissionsMap->at(this->geolocation) = true;
    }else{
        this->geolocation->setVisible(false);
        this->geolocation->setStyleSheet(
            "QPushButton{"
            "   background-color: rgb(52, 52, 52);"
            "   border-radius: 6px;"
            "   height: 35px;"
            "}"
        );
        this->permissionsMap->at(this->geolocation) = true;
    }

    if(permissions.at(QWebEnginePage::Feature::DesktopVideoCapture)){
        this->screenShare->setVisible(true);
        this->screenShare->setStyleSheet(
            "QPushButton{"
            "   background-color: rgb(81, 169, 79);"
            "   border-radius: 6px;"
            "   height: 35px;"
            "}"
        );
        this->permissionsMap->at(this->screenShare) = true;
    }else{
        this->screenShare->setVisible(false);
        this->screenShare->setStyleSheet(
            "QPushButton{"
            "   background-color: rgb(52, 52, 52);"
            "   border-radius: 6px;"
            "   height: 35px;"
            "}"
        );
        this->permissionsMap->at(this->screenShare) = true;
    }

    if(permissions.at(QWebEnginePage::Feature::MediaVideoCapture)){
        this->camera->setVisible(true);
        this->camera->setStyleSheet(
            "QPushButton{"
            "   background-color: rgb(81, 169, 79);"
            "   border-radius: 6px;"
            "   height: 35px;"
            "}"
        );
        this->permissionsMap->at(this->camera) = true;
    }else{
        this->camera->setVisible(false);
        this->camera->setStyleSheet(
            "QPushButton{"
            "   background-color: rgb(52, 52, 52);"
            "   border-radius: 6px;"
            "   height: 35px;"
            "}"
        );
        this->permissionsMap->at(this->camera) = true;
    }

    if(permissions.at(QWebEnginePage::Feature::MediaAudioCapture)){
        this->microphone->setVisible(true);
        this->microphone->setStyleSheet(
            "QPushButton{"
            "   background-color: rgb(81, 169, 79);"
            "   border-radius: 6px;"
            "   height: 35px;"
            "}"
        );
        this->permissionsMap->at(this->microphone) = true;
    }else{
        this->microphone->setVisible(false);
        this->microphone->setStyleSheet(
            "QPushButton{"
            "   background-color: rgb(52, 52, 52);"
            "   border-radius: 6px;"
            "   height: 35px;"
            "}"
        );
        this->permissionsMap->at(this->microphone) = true;
    }

    this->layout->update();
    this->updateGeometry();
}

PermissionsGroup::~PermissionsGroup() = default;

PageSettingsDialog::PageSettingsDialog(QWidget *parent): QDialog(parent), muted(false){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedWidth(300);

    int i = QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    QFont font(QFontDatabase::applicationFontFamilies(i).at(0), -1, QFont::Bold);

    this->layout = new QStackedLayout(this);

    this->mainPage = new QWidget();
    this->mainLayout = new QVBoxLayout(this->mainPage);
    this->mainLayout->setContentsMargins(15, 15, 15, 15);

    this->titleLayout = new QHBoxLayout();
    this->title = new QLabel("Document");
    this->title->setFont(font);
    this->title->setMaximumHeight(35);

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

    this->connect(this->permissions, &PermissionsGroup::toggleNotifications, this, [=](bool enabled){
        emit this->toggleNotifications(enabled);
    });

    this->connect(this->permissions, &PermissionsGroup::toggleCamera, this, [=](bool enabled){
        emit this->toggleCamera(enabled);
    });

    this->connect(this->permissions, &PermissionsGroup::toggleMicrophone, this, [=](bool enabled){
        emit this->toggleMicrophone(enabled);
    });

    this->connect(this->permissions, &PermissionsGroup::toggleGeolocation, this, [=](bool enabled){
        emit this->toggleGeolocation(enabled);
    });

    this->connect(this->permissions, &PermissionsGroup::toggleScreenShare, this, [=](bool enabled){
        emit this->toggleScreenShare(enabled);
    });

    this->connectionButton = new MenuButton(QPixmap(":/icons/white/secure.png"), "Connection is secure");
    this->cookiesButton = new MenuButton(QPixmap(":/icons/white/cookie.png"), "Cookies and site data");
    this->siteSettingsButton = new MenuButton(QPixmap(":/icons/white/settings.png"), "Site settings");

    this->mainLayout->addLayout(this->titleLayout);
    this->mainLayout->addWidget(this->soundButton);
    this->mainLayout->addWidget(this->permissions);
    this->mainLayout->addWidget(this->connectionButton);
    this->mainLayout->addWidget(this->cookiesButton);
    this->mainLayout->addWidget(this->siteSettingsButton);

    this->layout->addWidget(this->mainPage);
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

void PageSettingsDialog::setPermissions(std::map<QWebEnginePage::Feature, bool> permissions){
    this->permissions->setPermissions(permissions);
    this->updateGeometry();
}

void PageSettingsDialog::open(){
    this->move(this->parentWidget()->mapToGlobal(QPoint(this->parentWidget()->width()/2 - this->width()/2, 10)));
    QDialog::open();
}

PageSettingsDialog::~PageSettingsDialog() = default;