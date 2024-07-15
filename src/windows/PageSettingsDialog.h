#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPushButton>
#include <QPainter>
#include <QWebEnginePage>
#include <QSslCertificate>
#include <QStackedLayout>
#include "../components/IconButton.h"
#include "../components/Separator.h"

#include <vector>
#include <map>

class SoundButton: public QPushButton{
    Q_OBJECT
public:
    SoundButton(QWidget *parent=nullptr);
    ~SoundButton();
};

class MenuButton: public QWidget{
    Q_OBJECT
private:
    QHBoxLayout *layout;
    QLabel *icon, *text, *arrow;
public:
    MenuButton(QPixmap icon, QString text, QWidget *parent=nullptr);
    void setIcon(QPixmap icon);
    void setText(QString text);
    ~MenuButton();
protected:
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void clicked();
};

class PermissionsGroup: public QWidget{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    QPushButton *notifications, *geolocation, *microphone, *camera, *screenShare;
    std::map<QPushButton*, bool> *permissionsMap;

public:
    PermissionsGroup(QWidget *parent=nullptr);
    void setPermissions(std::map<QWebEnginePage::Feature, bool> permissions);
    ~PermissionsGroup();
signals:
    void toggleNotifications(bool enabled);
    void toggleGeolocation(bool enabled);
    void toggleMicrophone(bool enabled);
    void toggleCamera(bool enabled);
    void toggleScreenShare(bool enabled);
};

class SecurityPage: public QWidget{
    Q_OBJECT
private:
    QVBoxLayout *layout, *titlebarTextLayout, *certificateTextLayout;
    QHBoxLayout *titlebar, *securityLayout, *certificateLayout;
    IconButton *back;
    QLabel *title, *titleSubText, *security, *certificate, *certificateSubText, *securityIcon, *certificateIcon;
    Separator *separator;
public:
    SecurityPage(QWidget *parent=nullptr);
    void setUrl(QUrl url);
    void reset();
    ~SecurityPage();
signals:
    void backRequested();
};

class PageSettingsDialog: public QDialog{
    Q_OBJECT
private:
    QStackedLayout *layout;
    QWidget *mainPage;
    QVBoxLayout *mainLayout;
    QHBoxLayout *titleLayout;
    QLabel *title;
    SoundButton *soundButton;
    PermissionsGroup *permissions;
    MenuButton *connectionButton, *cookiesButton, *siteSettingsButton;
    SecurityPage *securityPage;
    Separator *separator;

    bool muted;
public:
    PageSettingsDialog(QWidget *parent=nullptr);
    void open();
    void reset();
    void setUrl(QUrl url);
    void setPermissions(std::map<QWebEnginePage::Feature, bool> permissions);
    ~PageSettingsDialog();
protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void toggleMuteAudio(bool mute);
    void toggleNotifications(bool enabled);
    void toggleGeolocation(bool enabled);
    void toggleMicrophone(bool enabled);
    void toggleCamera(bool enabled);
    void toggleScreenShare(bool enabled);
};