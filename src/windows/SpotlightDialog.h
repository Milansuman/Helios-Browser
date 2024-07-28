#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QWebChannel>

#include "../components/WebView.h"
#include "../api/TabApi.h"
#include "../api/FileApi.h"
#include "../api/DialogApi.h"

class SpotlightDialog: public QDialog{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    WebView *webview;
    int pos, group;

    QWebChannel *channel;
    TabsApi *tabsApi;
    FileApi *fileApi;
    DialogApi *dialogApi;
public:
    SpotlightDialog(QWidget *parent=nullptr);
    void open(int pos, int group);
    ~SpotlightDialog();
signals:
    void splitTabRequested(QUrl url);
    void splitTabHomeRequested();
    void newTabRequested(QUrl url);
    void splitTabFlipRequested();
    void addTabsRequested(QList<QList<QUrl>> tabsList);
};