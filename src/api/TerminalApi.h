#pragma once

#include <QObject>

class TerminalApi: public QObject{
    Q_OBJECT
public:
    TerminalApi(QObject *parent=nullptr);
    ~TerminalApi();
};