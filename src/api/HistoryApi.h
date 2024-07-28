#pragma once

#include <QObject>
#include <QWebEngineHistory>
#include <QJsonDocument>

/*
window.tabHistory.getHistory() : Array<{ title: string, url: string}>
*/

class HistoryApi: public QObject{
    Q_OBJECT
private:
    QWebEngineHistory *history;
public:
    HistoryApi(QWebEngineHistory *history, QObject *parent=nullptr);
    Q_INVOKABLE QJsonDocument getHistory();
    ~HistoryApi();
};