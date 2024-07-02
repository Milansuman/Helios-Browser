#include "ScreenShareDialog.h"

#include <QPainter>

ScreenShareDialog::ScreenShareDialog(QWidget *parent): QDialog(parent), request(nullptr){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMinimumSize(500, 400);
    this->layout = new QVBoxLayout(this);

    this->optionsWidget = new QTabWidget();
    this->optionsWidget->setStyleSheet(
        "QTabWidget::pane{"
        "   border: none;"
        "   background-color: rgb(30, 30, 30);"
        "}"
        "QTabBar::tab{"
        "   border: none;"
        "   background-color: rgb(30, 30, 30);"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "}"
        "QTabBar::tab:selected{"
        "   border: none;"
        "   border-radius: 5px;"
        "   background-color: rgb(50, 50, 50);"
        "}"
        "QTabBar::tab:!selected{"
        "   border: none;"
        "   border-radius: 5px;"
        "   background-color: rgb(30, 30, 30);"
        "}"
    );

    this->screensWidget = new QWidget();
    this->screensLayout = new QGridLayout(this->screensWidget);
    this->screensLayout->setHorizontalSpacing(5);
    this->screensLayout->setVerticalSpacing(5);
    this->optionsWidget->addTab(this->screensWidget, "Screens");

    this->windowsWidget = new QWidget();
    this->windowsLayout = new QGridLayout(this->windowsWidget);
    this->windowsLayout->setVerticalSpacing(5);
    this->windowsLayout->setHorizontalSpacing(5);
    this->optionsWidget->addTab(this->windowsWidget, "Windows");

    this->layout->addWidget(this->optionsWidget);
}

void ScreenShareDialog::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(30, 30, 30)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
    QDialog::paintEvent(event);
}

void ScreenShareDialog::addScreen(QModelIndex screen){
    PreviewWindow *screenWindow = new PreviewWindow(screen.data(Qt::DisplayRole).toString());

    this->connect(screenWindow, &PreviewWindow::clicked, this, [=](){
        this->request->selectScreen(screen);
        this->accept();
    });

    this->screens.push_back(screenWindow);
    this->screensLayout->addWidget(screenWindow, this->screens.size()/3, this->screens.size()%3);
}

void ScreenShareDialog::addWindow(QModelIndex window){
    PreviewWindow *windowWindow = new PreviewWindow(window.data(Qt::DisplayRole).toString());

    this->connect(windowWindow, &PreviewWindow::clicked, this, [=](){
        this->request->selectWindow(window);
        this->accept();
    });

    this->windows.push_back(windowWindow);
    this->windowsLayout->addWidget(windowWindow, this->windows.size()/3, this->windows.size()%3);
}

void ScreenShareDialog::exec(const QWebEngineDesktopMediaRequest &request){
    this->request = &request;
    for(int i=0; i<request.screensModel()->rowCount(); i++){
        this->addScreen(request.screensModel()->index(i));
    }

    for(int i=0; i<request.windowsModel()->rowCount(); i++){
        this->addWindow(request.windowsModel()->index(i));
    }

    QDialog::exec();
}

ScreenShareDialog::~ScreenShareDialog(){
    for(PreviewWindow *screen: this->screens){
        delete screen;
    }

    for(PreviewWindow *window: this->windows){
        delete window;
    }

    this->screens.clear();
    this->windows.clear();
}