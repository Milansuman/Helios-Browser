#include <QApplication>
#include "browserwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BrowserWindow *window = new BrowserWindow(nullptr, 800.0, 600.0);
    window->show();
    return a.exec();
}
