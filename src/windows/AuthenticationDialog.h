#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QUrl>
#include <QPaintEvent>
#include <QAuthenticator>
#include <QPushButton>

class AuthenticationDialog: public QDialog {
    Q_OBJECT
private:
    QLabel *usernameLabel, *passwordLabel, *infoLabel;
    QLineEdit *username, *password;
    QPushButton *cancel, *submit;
    QVBoxLayout *layout;
    QAuthenticator *authenticator;
public:
    AuthenticationDialog(QWidget *parent=nullptr);
    void exec(const QUrl &requestUrl, QAuthenticator *authenticator);
    ~AuthenticationDialog();
protected:
    void paintEvent(QPaintEvent *event) override;
};