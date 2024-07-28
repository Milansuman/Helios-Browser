#include "FileApi.h"

#include <QDir>
#include <QJsonArray>
#include <QDirIterator>
#include <QFile>
#include <QStringList>
#include <QFileDialog>
#include <QJsonObject>

FileApi::FileApi(QObject *parent): QObject(parent){}

QJsonDocument FileApi::listDir(QString path, bool subdir){
    QJsonDocument folderData;

    QDir directory(path);
    directory.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QJsonArray entries;
    for(QFileInfo path: directory.entryInfoList()){
        QJsonObject fileData;
        fileData.insert("path", path.filePath());
        fileData.insert("isDir", path.isDir());

        if(subdir && path.isDir()){
            QJsonArray subdirEntries;
            for(QFileInfo subPath: QDir(path.filePath()).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)){
                QJsonObject subFileData;

                subFileData.insert("path", subPath.filePath());
                subFileData.insert("isDir", subPath.isDir());
                subdirEntries.append(subFileData);
            }
            fileData.insert("dirs", subdirEntries);
        }

        entries.append(fileData);
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