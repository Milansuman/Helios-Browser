#pragma once

#include <QObject>
#include <QJsonDocument>

class SearchSuggestionsApi: public QObject{
    Q_OBJECT
public:
    SearchSuggestionsApi(QObject *parent=nullptr);
    Q_INVOKABLE void getDuckDuckGoSuggestions(QString text);
    ~SearchSuggestionsApi();
signals:
    void duckDuckGoSuggestionsGenerated(QJsonDocument results);
};