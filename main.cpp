#include <QApplication>
#include <QFontDatabase>
#include <QFont>
#include "browserwindow.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resources);
    BrowserWindow *browser = new BrowserWindow(nullptr, 800.0, 600.0);

    QFontDatabase::addApplicationFont(":/fonts/calsans.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont *font = new QFont(fontFamily);

    browser->setFont(*font);
    browser->show();
    return a.exec();
}
