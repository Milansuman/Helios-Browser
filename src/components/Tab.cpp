#include "Tab.h"

#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QClipboard>
#include <QWebEnginePage>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineScript>
#include <QSizePolicy>
#include <QWebEngineScriptCollection>
#include <QAction>

Tab::Tab(QWebEngineProfile *profile, QWidget *parent): Tab(profile, "https://duckduckgo.com/", parent){}

Tab::Tab(QWebEngineProfile *profile, QString url, QWidget *parent): QWidget(parent), fullScreenWindow(nullptr){
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(0,0,0,0);
    this->layout->setSpacing(0);
    this->setMouseTracking(true);

    this->webview = new WebView(profile);
    this->webview->load(QUrl(url));

    this->devtoolsSplitter = new QSplitter();
    this->devtoolsSplitter->setMouseTracking(true);

    this->searchDialog = new SearchDialog(this);
    this->authDialog = new AuthenticationDialog(this);
    //this->initCustomScrollBar();
    this->tabTitleBar = new TabTitleBar();

    this->connect(this->webview, &WebView::loadStarted, this, [=](){
        this->tabTitleBar->setTitle(this->webview->url().toString());
        //this->initCustomScrollBar();
    });

    this->connect(this->webview, &WebView::loadFinished, this, [=](){
        this->tabTitleBar->setTitle(this->webview->title());
        emit this->titleChanged(this->webview->title());
        //this->initCustomScrollBar();
    });

    this->connect(this->webview, &WebView::iconChanged, this, [=](){
        emit this->iconChanged(this->webview->icon());
    });

    this->connect(this->webview, &WebView::colorChanged, this, [=](QColor color){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, color);
        setPalette(palette);

        double luminance = (0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue()) / 255;

        this->tabTitleBar->setIsBlack(luminance > 0.5);
    });

    this->connect(this->webview->page(), &QWebEnginePage::fullScreenRequested, this, [=](QWebEngineFullScreenRequest fullScreenRequest){
        if(fullScreenRequest.toggleOn()){
            if(this->fullScreenWindow){
                return;
            }
            fullScreenRequest.accept();
            this->fullScreenWindow = new FullScreenWindow(profile, this->webview);
        }else{
            if (!this->fullScreenWindow)
                return;
            fullScreenRequest.accept();
            delete this->fullScreenWindow;
            this->fullScreenWindow = nullptr;
        }
    });

    this->connect(this->webview->page(), &QWebEnginePage::authenticationRequired, this, [=](const QUrl &requestUrl, QAuthenticator *authenticator){
        this->authDialog->exec(requestUrl, authenticator);
    });

    this->connect(this->tabTitleBar, &TabTitleBar::searchRequested, this, [=](){
        this->searchDialog->open();
    });

    this->connect(this->searchDialog, &SearchDialog::accepted, this, [=](){
        this->webview->load(QUrl(this->searchDialog->getSearch()));
    });

    this->connect(this->tabTitleBar, &TabTitleBar::copyLinkRequested, this, [=](){
        qApp->clipboard()->setText(this->webview->url().toString());
    });

    this->connect(this->tabTitleBar, &TabTitleBar::previousPageRequested, this, [=](){
        this->webview->back();
    });

    this->connect(this->tabTitleBar, &TabTitleBar::nextPageRequested, this, [=](){
        this->webview->forward();
    });

    this->connect(this->tabTitleBar, &TabTitleBar::reloadRequested, this, [=](){
        this->webview->reload();
    });

    this->connect(this->tabTitleBar, &TabTitleBar::splitTabLeftRequested, this, [=](){
        emit this->splitTabLeftRequested();
    });

    this->connect(this->tabTitleBar, &TabTitleBar::splitTabRightRequested, this, [=](){
        emit this->splitTabRightRequested();
    });

    this->connect(this->tabTitleBar, &TabTitleBar::closeTabRequested, this, [=](){
        emit this->closeTabRequested();
    });

    this->layout->addWidget(this->tabTitleBar);
    //this->layout->addWidget(this->pageSurface);

    this->devtoolsSplitter->addWidget(this->webview);
    this->layout->addWidget(this->devtoolsSplitter);
}

//adding a custom scroll bar which gets hidden when not in use
void Tab::initCustomScrollBar() {
    QWebEngineScript script;
    script.setName("customScrollBarScript");
    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setWorldId(QWebEngineScript::ApplicationWorld);
    script.setRunsOnSubFrames(true);

    QString js = R"(
        var style = document.createElement('style');
        style.innerHTML = `
            ::-webkit-scrollbar {
                width: 10px;
            }

            ::-webkit-scrollbar-track {
                background-color: transparent;
            }

            ::-webkit-scrollbar-thumb {
                background: linear-gradient(to right, transparent 60%, #444 40%);
            }

            ::-webkit-scrollbar-thumb:hover {
                background: #888;
                border-radius: 6px;
            }
        `;
        document.head.appendChild(style);
    )";

    script.setSourceCode(js);
    this->webview->page()->scripts().insert(script);
}

void Tab::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(palette().window());
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
}

void Tab::resizeEvent(QResizeEvent *event){
    // QPainterPath path;
    // path.addRoundedRect(rect(), 10, 10);
    // this->setMask(path.toFillPolygon().toPolygon());
}

void Tab::mousePressEvent(QMouseEvent *event){
    emit this->tabFocused();
}

void Tab::setTitleBarVisible(bool visible){
    this->tabTitleBar->setVisible(visible);
}

void Tab::requestSearchDialog(){
    this->searchDialog->open();
}

QString Tab::getTitle(){
    return this->webview->title();
}

QIcon Tab::getIcon(){
    return this->webview->icon();
}

void Tab::copyUrl(){
    qApp->clipboard()->setText(this->webview->url().toString());
}

void Tab::requestNextPage(){
    this->webview->forward();
}

void Tab::requestPreviousPage(){
    this->webview->back();
}

void Tab::requestReload(){
    this->webview->reload();
}

void Tab::openDevTools(){
    this->devtools = new WebView();
    this->webview->page()->setDevToolsPage(this->devtools->page());
    this->devtoolsSplitter->insertWidget(1, this->devtools);

    this->connect(this->devtools->page(), &QWebEnginePage::windowCloseRequested, this, [=](){
        this->closeDevTools();
    });
}

void Tab::closeDevTools(){
    delete this->devtools;
}

Tab::~Tab(){
    delete this->webview;
    delete this->tabTitleBar;
    delete this->layout;
    delete this->searchDialog;
}
