#include "FileApi.h"

#include <QDir>
#include <QJsonArray>
#include <QDirIterator>
#include <QFile>
#include <QStringList>
#include <QFileDialog>

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

QJsonDocument FileApi::search(QString fuzz){
    QJsonDocument filesData;

    QDirIterator iter(QDir::homePath(), QStringList(fuzz), QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    QJsonArray files;
    while(iter.hasNext()){
        QFile file(iter.next());
        files.append(file.fileName());
    }

    filesData.setArray(files);
    return filesData;
}

QString FileApi::getFileURL(){
    return QFileDialog::getOpenFileUrl(nullptr, "Open a file", QDir::homePath(), "").toString();
}

QString FileApi::getFolderURL(){
    return QFileDialog::getExistingDirectoryUrl(nullptr, "Open a file", QDir::homePath()).toString();
}

FileApi::~FileApi() = default;