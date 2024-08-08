#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QWebChannel>

#include "../components/WebView.h"
#include "../api/TabApi.h"
#include "../api/FileApi.h"
#include "../api/DialogApi.h"
#include "../api/OllamaApi.h"
#include "../api/SearchSuggestionsApi.h"

class SpotlightDialog: public QDialog{
    Q_OBJECT
    Q_PROPERTY(int tab READ getTab NOTIFY tabChanged)
    Q_PROPERTY(int group READ getGroup NOTIFY groupChanged)
private:
    QVBoxLayout *layout;
    WebView *webview;
    int m_pos, m_group;

    QWebChannel *channel;
    TabsApi *tabsApi;
    FileApi *fileApi;
    DialogApi *dialogApi;
    OllamaApi *ollamaApi;
    SearchSuggestionsApi *searchSuggestionsApi;
public:
    SpotlightDialog(QWidget *parent=nullptr);
    void open(int pos, int group);
    int getTab();
    int getGroup();
    ~SpotlightDialog();
protected:
    void paintEvent(QPaintEvent *event);
signals:
    void splitTabRequested(QUrl url);
    void splitTabHomeRequested();
    void newTabRequested(QUrl url);
    void splitTabFlipRequested();
    void addTabsRequested(QList<QList<QUrl>> tabsList);
    void loadUrl(int group, int tab, QString url);
    void tabChanged(int tab);
    void groupChanged(int group);
};