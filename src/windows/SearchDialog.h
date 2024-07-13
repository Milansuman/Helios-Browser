#pragma once

#include <QDialog>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

#ifdef _WIN32
#include <windows.h>
#endif

class SearchDialog : public QDialog
{
    Q_OBJECT
private:
    QString searchText;
    QLabel *searchIconLabel;
    QLineEdit *searchbar;
    QHBoxLayout *searchLayout;
#ifdef _WIN32
    void enableBlurBehind();
#endif
public:
    SearchDialog(QWidget *parent = nullptr);
    QString getSearch();
    void setUrl(QUrl url);
    // #ifdef __linux__
    void open();
    // #endif
    ~SearchDialog();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};