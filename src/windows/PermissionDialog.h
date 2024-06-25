#pragma once

#include <QDialog>
#include <QPaintEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class PermissionDialog : public QDialog{
    Q_OBJECT
private:
    QLabel *messageLabel;
    QVBoxLayout *dialogLayout;
    QPushButton *ok, *cancel;
public:
    PermissionDialog(QWidget *parent=nullptr);
    void exec(QString message);
    ~PermissionDialog();
protected:
    void paintEvent(QPaintEvent *event) override;
};