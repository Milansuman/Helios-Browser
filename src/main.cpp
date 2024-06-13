#include <QApplication>

#include "windows/BrowserWindow.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    
    BrowserWindow *window = new BrowserWindow(QSize(1300, 800));
    window->show();

    app.exec();
}
