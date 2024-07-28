#pragma once

#include <QObject>

class DialogApi: public QObject{
    Q_OBJECT
public:
    DialogApi(QObject *parent=nullptr);
    Q_INVOKABLE void closeDialog();
    ~DialogApi();
signals:
    void closeDialogRequested();
};