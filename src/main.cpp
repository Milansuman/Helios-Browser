#include <QApplication>

#include <QFontDatabase>
#include <QFont>

#include "windows/BrowserWindow.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/SFUIText-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont *font = new QFont(fontFamily);
    
    BrowserWindow *window = new BrowserWindow(QSize(1300, 800));
    window->show();
    window->setFont(*font);

    app.exec();
}
