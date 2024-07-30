#include "OllamaApi.h"

#include <QPromise>
#include <QFuture>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonObject>

OllamaApi::OllamaApi(QObject *parent): QObject(parent){}

void OllamaApi::generate(QString model, QString prompt){
    QJsonDocument params;
    QJsonObject paramsObject;

    paramsObject.insert("model", model);
    paramsObject.insert("prompt", prompt);
    paramsObject.insert("stream", false);

    params.setObject(paramsObject);
    QNetworkRequest request(QUrl("http://127.0.0.1:11434/api/generate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    this->manager = new QNetworkAccessManager(this);
    this->manager->post(request, params.toJson());

    this->connect(this->manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply){
        QByteArray responseData = reply->readAll();
        QJsonDocument result = QJsonDocument::fromJson(responseData);
        
        emit this->responseGenerated(result);
        reply->deleteLater();
    });
}

OllamaApi::~OllamaApi() = default;