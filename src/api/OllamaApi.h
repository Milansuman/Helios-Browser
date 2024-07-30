#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>

class OllamaApi: public QObject{
    Q_OBJECT
private:
    QNetworkAccessManager *manager;
public:
    OllamaApi(QObject *parent=nullptr);
    Q_INVOKABLE void generate(QString model, QString prompt);
    ~OllamaApi();
signals:
    void responseGenerated(QJsonDocument response);
};