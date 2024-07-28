#include "HistoryApi.h"

#include <QWebEngineHistoryItem>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

HistoryApi::HistoryApi(QWebEngineHistory *history, QObject *parent): QObject(parent), history(history){}

QJsonDocument HistoryApi::getHistory(){
    QJsonDocument result;
    QJsonArray array;
    for(QWebEngineHistoryItem item: this->history->items()){
        QJsonObject historyItem;
        historyItem.insert(item.title(), QJsonValue(item.url().toString()));
        array.append(historyItem);
    }
    result.setArray(array);

    return result;
}

HistoryApi::~HistoryApi() = default;