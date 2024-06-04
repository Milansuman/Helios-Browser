#pragma once

#include <QMainWindow>

class TestWindow: public QMainWindow{
    Q_OBJECT
public:
    TestWindow(QWidget *parent=nullptr): QMainWindow(parent){
        this->setWindowTitle("hello uwu");
        this->setMinimumSize(800, 600);
    }
};