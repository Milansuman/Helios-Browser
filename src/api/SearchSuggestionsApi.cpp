#include "SearchSuggestionsApi.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>

SearchSuggestionsApi::SearchSuggestionsApi(QObject *parent): QObject(parent){}

void SearchSuggestionsApi::getDuckDuckGoSuggestions(QString text){
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    manager->get(QNetworkRequest(QUrl("https://duckduckgo.com/ac/?q=" + text)));

    this->connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply){
        QByteArray response = reply->readAll();

        emit this->duckDuckGoSuggestionsGenerated(QJsonDocument::fromJson(response));
        reply->deleteLater();
    });
}

SearchSuggestionsApi::~SearchSuggestionsApi() = default;