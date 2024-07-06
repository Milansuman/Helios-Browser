#include "PageSettings.h"

#include <QFontDatabase>
#include <QFont>

SoundButton::SoundButton(QWidget *parent): QPushButton(parent), isSoundOn(true){
    this->setStyleSheet(
        "QPushButton{"
        "   background-color: rgb(228, 228, 228);"
        "   border-radius: 10px;"
        "   border: none;"
        "}"
    );
    this->setFixedWidth(150);
    this->setText("Sound");
}

SoundButton::~SoundButton() = default;

PermissionsAction::PermissionsAction(QObject *parent): QWidgetAction(parent){
    this->defaultWidget = new QWidget();

    int i = QFontDatabase::addApplicationFont(":/fonts/calsans.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(i).at(0);
    QFont *font = new QFont("Cal Sans");

    this->layout = new QVBoxLayout(this->defaultWidget);
    this->titleLayout = new QHBoxLayout();
    this->urlLabel = new QLabel("Document");
    this->urlLabel->setFont(*font);

    // this->titleLayout->addStretch();
    // this->titleLayout->addWidget(this->urlLabel);
    // this->titleLayout->addStretch();

    this->soundButton = new SoundButton();

    //this->layout->addLayout(this->titleLayout);
    this->layout->addWidget(this->soundButton);
}

void PermissionsAction::setTitle(QString title){
    //this->urlLabel->setText(title);
}

PermissionsAction::~PermissionsAction() = default;

PageSettings::PageSettings(QWidget *parent): IconButton(":/icons/white/page-settings.png", parent){
    this->menu = new QMenu();
    this->menu->setStyleSheet(
        "QMenu{"
        "   border: none;"
        "   background-color: rgb(30, 30, 30);"
        "   border-radius: 10;"
        "}"
    );

    this->permissionsAction = new PermissionsAction();
    this->menu->addAction(this->permissionsAction);

    this->connect(this, &IconButton::clicked, this, [=](){
        this->menu->popup(this->parentWidget()->mapToGlobal(this->parentWidget()->geometry().center()) + QPoint(-(this->menu->sizeHint().width()/2), 25));
    });
}

void PageSettings::setTitle(QString title){
    this->permissionsAction->setText(title);
}

PageSettings::~PageSettings() = default;