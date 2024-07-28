#pragma once

#include <QObject>
#include <QJsonDocument>

/*
listDir(string path, boolean subdir?) : Array<string>
getFileUrl(): string
getFolderUrl(): string
*/

class FileApi: public QObject{
    Q_OBJECT
public:
    FileApi(QObject *parent=nullptr);
    Q_INVOKABLE QJsonDocument listDir(QString path, bool subdir=false);
    QJsonDocument search(QString fuzz);
    Q_INVOKABLE QString getFileURL();
    Q_INVOKABLE QString getFolderURL();
    ~FileApi();
};