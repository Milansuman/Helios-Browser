#include "FullScreenWindow.h"

#include <QAction>

FullScreenWindow::FullScreenWindow(QWebEngineProfile *profile, WebView *view, QWidget *parent): QWidget(parent), previousView(view){
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QAction *exitAction = new QAction(this);
    exitAction->setShortcut(Qt::Key_Escape);
    connect(exitAction, &QAction::triggered, [this]() {
        this->fullScreenView->triggerPageAction(QWebEnginePage::ExitFullScreen);
    });
    addAction(exitAction);

    this->fullScreenView = new WebView(profile, this);
    this->fullScreenView->setPage(this->previousView->page());

    this->setGeometry(view->window()->geometry());
    this->showFullScreen();
}

void FullScreenWindow::resizeEvent(QResizeEvent *event){
    QRect viewGeometry(QPoint(0, 0), this->size());
    this->fullScreenView->setGeometry(viewGeometry);
    QWidget::resizeEvent(event);
}

FullScreenWindow::~FullScreenWindow(){
    this->previousView->setPage(this->fullScreenView->page());
    this->hide();
}