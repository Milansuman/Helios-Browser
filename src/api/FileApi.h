#pragma once

#include <QObject>
#include <QJsonDocument>

/*
listDir(string path) : Array<string>
*/

class FileApi: public QObject{
    Q_OBJECT
public:
    FileApi(QObject *parent=nullptr);
    Q_INVOKABLE QJsonDocument listDir(QString path);
    ~FileApi();
};