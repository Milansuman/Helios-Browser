#include "FileApi.h"

#include <QDir>
#include <QJsonArray>

FileApi::FileApi(QObject *parent): QObject(parent){}

QJsonDocument FileApi::listDir(QString path){
    QJsonDocument folderData;

    QDir directory(path);
    QJsonArray entries;
    for(QString path: directory.entryList()){
        entries.append(path);
    }

    folderData.setArray(entries);
    return folderData;
}

FileApi::~FileApi() = default;