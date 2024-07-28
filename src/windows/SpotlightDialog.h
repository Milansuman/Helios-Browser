#pragma once

#include <QDialog>
#include <QVBoxLayout>

#include "../components/WebView.h"

class SpotlightDialog: public QDialog{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    WebView *webview;
    int pos, group;
public:
    SpotlightDialog(QWidget *parent=nullptr);
    void open(int pos, int group);
    ~SpotlightDialog();
};