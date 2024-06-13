#pragma once

#include <QDialog>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

class SearchDialog: public QDialog{
    Q_OBJECT
private:
    QString searchText;
    QLabel *searchIconLabel;
    QLineEdit *searchbar;
    QHBoxLayout *searchLayout;
public:
    SearchDialog(QWidget *parent = nullptr);
    QString getSearch();
    #ifdef __linux__
    void open();
    #endif
    ~SearchDialog();
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};