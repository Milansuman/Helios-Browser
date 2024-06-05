#include <QApplication>

#include "windows/BrowserWindow.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    
    BrowserWindow *window = new BrowserWindow(QSize(800, 600));
    window->show();

    app.exec();
}