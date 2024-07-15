#pragma once

#include <QDialog>
#include <QWebEngineCertificateError>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class CertificateErrorDialog: public QDialog{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    QPushButton *ok, *cancel;
    QLabel *title, *description;
public:
    CertificateErrorDialog(QWidget *parent=nullptr);
    void exec(QWebEngineCertificateError certificateError);
    ~CertificateErrorDialog();
protected:
    void paintEvent(QPaintEvent *event) override;
};