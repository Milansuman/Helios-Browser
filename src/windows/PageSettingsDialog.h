#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPushButton>
#include <QPainter>

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
public:
    PermissionsGroup(QWidget *parent=nullptr);
    ~PermissionsGroup();
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
    ~PageSettingsDialog();
protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void toggleMuteAudio(bool mute);
};