#include <QApplication>

#include "windows/testwindow.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    
    TestWindow *window = new TestWindow();
    window->show();

    app.exec();
}