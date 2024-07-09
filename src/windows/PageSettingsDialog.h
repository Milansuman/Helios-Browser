#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPushButton>
#include <QPainter>
#include <QWebEnginePage>

#include <vector>
#include <map>

class SoundButton: public QPushButton{
    Q_OBJECT
public:
    SoundButton(QWidget *parent=nullptr);
    ~SoundButton();
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

class PageSettingsDialog: public QDialog{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    QHBoxLayout *titleLayout;
    QLabel *title;
    SoundButton *soundButton;
    PermissionsGroup *permissions;

    bool muted;
public:
    PageSettingsDialog(QWidget *parent=nullptr);
    void open();
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