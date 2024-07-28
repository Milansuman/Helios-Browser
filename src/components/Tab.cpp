#include "Tab.h"

#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QClipboard>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineDownloadRequest>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineScript>
#include <QSizePolicy>
#include <QWebEngineScriptCollection>
#include <QWebEngineNewWindowRequest>
#include <QWebEngineDesktopMediaRequest>
#include <QWebEngineClientCertificateSelection>
#include <QWebEngineCertificateError>
#include <QAction>
#include <QFileInfo>
#include <algorithm>
#include <QWebChannel>

Tab::Tab(QWebEngineProfile *profile, QWidget *parent) : Tab(profile, "https://fluxbrowserhome.netlify.app/", parent) {}

Tab::Tab(QWebEngineProfile *profile, QString url, QWidget *parent) : QWidget(parent), fullScreenWindow(nullptr), devtools(nullptr), screenShareDialog(nullptr), profile(profile)
{
    this->permissions = new std::map<QWebEnginePage::Feature, bool>({{QWebEnginePage::Feature::Notifications, false},
                                                                     {QWebEnginePage::Feature::Geolocation, false},
                                                                     {QWebEnginePage::Feature::DesktopVideoCapture, false},
                                                                     {QWebEnginePage::Feature::MediaAudioCapture, false},
                                                                     {QWebEnginePage::Feature::MediaVideoCapture, false},
                                                                     {QWebEnginePage::Feature::MediaAudioVideoCapture, false},
                                                                     {QWebEnginePage::Feature::MouseLock, true}});
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(0, 0, 0, 0);
    this->layout->setSpacing(0);
    this->setMouseTracking(true);

    this->progressIndicator = new QProgressBar();
    this->progressIndicator->setTextVisible(false);
    this->progressIndicator->setFixedHeight(3);
    this->progressIndicator->setStyleSheet(
        "QProgressBar{"
        "   border-radius: 0px;"
        "   background-color: rbga(0,0,0,0);"
        "   margin: 0px;"
        "   padding: 0px;"
        "}"
        "QProgressBar::chunk{"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                               stop:0 #218B91,"
        "                               stop:1 #A123AC);"
        "}");

    this->downloadManager = new DownloadManager(this);

    this->webview = new WebView(profile);
    this->webview->load(QUrl(url));

    this->channel = new QWebChannel(this->webview->page());
    this->webview->page()->setWebChannel(this->channel);

    this->tabsApi = new TabsApi();
    this->channel->registerObject("tabs", this->tabsApi);

    this->connect(this->tabsApi, &TabsApi::splitTabRequested, this, [=](QUrl url)
                  { emit this->splitTabRequested(url); });

    this->connect(this->tabsApi, &TabsApi::splitTabHomeRequested, this, [=]()
                  { emit this->splitTabRightRequested(); });

    this->connect(this->tabsApi, &TabsApi::newTabRequested, this, [=](QUrl url)
                  { emit this->newTabRequested(url); });

    this->connect(this->tabsApi, &TabsApi::splitTabFlipRequested, this, [=]()
                  { emit this->splitTabFlipRequested(); });

    this->historyApi = new HistoryApi(this->webview->page()->history());
    this->channel->registerObject("tabHistory", this->historyApi);

    this->fileApi = new FileApi();
    this->channel->registerObject("fs", this->fileApi);

    QWebEngineScript script;
    script.setName("WebChannelScript");
    script.setSourceCode(R"(
        var script = document.createElement('script');
        script.src = 'qrc:///qtwebchannel/qwebchannel.js';

        script.onload = function() {
            new QWebChannel(qt.webChannelTransport, function(channel) {
                window.tabs = channel.objects.tabs;
                window.tabHistory = channel.objects.tabHistory;
                window.fs = channel.objects.fs;
            });
        };
        document.getElementsByTagName('head')[0].appendChild(script);
    )");

    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setWorldId(QWebEngineScript::MainWorld);
    script.setRunsOnSubFrames(true);

    this->webview->page()->scripts().insert(script);

    this->devtoolsSplitter = new QSplitter();
    this->devtoolsSplitter->setMouseTracking(true);

    // this->searchDialog = new SearchDialog(this);
    this->authDialog = new AuthenticationDialog(this);
    this->permissionDialog = new PermissionDialog(this);
    this->certificateErrorDialog = new CertificateErrorDialog(this);
    // this->initCustomScrollBar();
    this->tabTitleBar = new TabTitleBar();

    this->pageSettingsDialog = new PageSettingsDialog(this->webview);

    this->connect(this->pageSettingsDialog, &PageSettingsDialog::toggleMuteAudio, this, [=](bool muted)
                  { this->webview->page()->setAudioMuted(muted); });

    this->connect(this->pageSettingsDialog, &PageSettingsDialog::toggleCamera, this, [=](bool enabled)
                  {
        QUrl url = this->webview->url();
        QString securityOrigin = url.scheme() + "://" + url.host();
        if (url.port() != -1) {
            securityOrigin += ":" + QString::number(url.port());
        }
        this->permissions->at(QWebEnginePage::Feature::MediaVideoCapture) = false;
        emit this->webview->page()->featurePermissionRequested(url, QWebEnginePage::Feature::MediaVideoCapture); });

    this->connect(this->pageSettingsDialog, &PageSettingsDialog::toggleNotifications, this, [=](bool enabled)
                  {
        QUrl url = this->webview->url();
        QString securityOrigin = url.scheme() + "://" + url.host();
        if (url.port() != -1) {
            securityOrigin += ":" + QString::number(url.port());
        }
        this->permissions->at(QWebEnginePage::Feature::Notifications) = enabled;
        emit this->webview->page()->featurePermissionRequested(url, QWebEnginePage::Feature::Notifications); });

    this->connect(this->pageSettingsDialog, &PageSettingsDialog::toggleGeolocation, this, [=](bool enabled)
                  {
        QUrl url = this->webview->url();
        QString securityOrigin = url.scheme() + "://" + url.host();
        if (url.port() != -1) {
            securityOrigin += ":" + QString::number(url.port());
        }
        this->permissions->at(QWebEnginePage::Feature::Geolocation) = enabled;
        emit this->webview->page()->featurePermissionRequested(url, QWebEnginePage::Feature::Geolocation); });

    this->connect(this->pageSettingsDialog, &PageSettingsDialog::toggleMicrophone, this, [=](bool enabled)
                  {
        QUrl url = this->webview->url();
        QString securityOrigin = url.scheme() + "://" + url.host();
        if (url.port() != -1) {
            securityOrigin += ":" + QString::number(url.port());
        }
        this->permissions->at(QWebEnginePage::Feature::MediaAudioCapture) = enabled;
        emit this->webview->page()->featurePermissionRequested(url, QWebEnginePage::Feature::MediaAudioCapture); });

    this->connect(this->pageSettingsDialog, &PageSettingsDialog::toggleScreenShare, this, [=](bool enabled)
                  {
        QUrl url = this->webview->url();
        QString securityOrigin = url.scheme() + "://" + url.host();
        if (url.port() != -1) {
            securityOrigin += ":" + QString::number(url.port());
        }
        this->permissions->at(QWebEnginePage::Feature::DesktopVideoCapture) = enabled;
        emit this->webview->page()->featurePermissionRequested(url, QWebEnginePage::Feature::DesktopVideoCapture); });

    this->connect(this->webview, &WebView::loadStarted, this, [=]()
                  {
                      this->tabTitleBar->setTitle(this->webview->url().toString());
                      this->pageSettingsDialog->reset();
                      // this->initCustomScrollBar();
                  });

    this->connect(this->webview, &WebView::loadFinished, this, [=]()
                  {
        this->tabTitleBar->setTitle(this->webview->title());
        emit this->titleChanged(this->webview->title());
        this->progressIndicator->setVisible(false);
        this->pageSettingsDialog->setUrl(this->webview->url());
        //this->searchDialog->setUrl(this->webview->url());
        
        for(std::pair<QWebEnginePage::Feature, bool> pair: *this->permissions){
            this->permissions->at(pair.first) = false;
        }

        this->pageSettingsDialog->setPermissions(*this->permissions); });

    this->connect(this->webview, &WebView::loadProgress, this, [=](int progress)
                  {
        if(progress == 100){
            this->tabTitleBar->setTitle(this->webview->title());
            emit this->titleChanged(this->webview->title());
            this->progressIndicator->setVisible(false);

            for(std::pair<QWebEnginePage::Feature, bool> pair: *this->permissions){
                this->permissions->at(pair.first) = false;
            }

            this->pageSettingsDialog->setPermissions(*this->permissions);
        }else{
            this->progressIndicator->setVisible(true);
        }
        this->progressIndicator->setValue(progress); });

    this->connect(this->webview, &WebView::iconChanged, this, [=]()
                  { emit this->iconChanged(this->webview->icon()); });

    this->connect(this->webview, &WebView::colorChanged, this, [=](QColor color)
                  {
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, color);
        setPalette(palette);

        double luminance = (0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue()) / 255;

        this->tabTitleBar->setIsBlack(luminance > 0.5); });

    this->connect(this->webview->page(), &QWebEnginePage::fullScreenRequested, this, [=](QWebEngineFullScreenRequest fullScreenRequest)
                  {
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
        } });

    this->connect(this->webview->page(), &QWebEnginePage::authenticationRequired, this, [=](const QUrl &requestUrl, QAuthenticator *authenticator)
                  { this->authDialog->exec(requestUrl, authenticator); });

    this->connect(this->webview->page(), &QWebEnginePage::featurePermissionRequested, this, [=](const QUrl &securityOrigin, QWebEnginePage::Feature feature)
                  {
        if(this->permissions->at(feature)){
            this->webview->page()->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
        }else{
            switch(feature){
                case QWebEnginePage::Feature::MediaAudioCapture:
                    this->permissionDialog->exec(this->webview->url().host() + " is requesting microphone access.");
                    break;
                case QWebEnginePage::Feature::Notifications:
                    this->permissionDialog->exec(this->webview->url().host() + " wants to send notifications.");
                    break;
                case QWebEnginePage::Feature::Geolocation:
                    this->permissionDialog->exec(this->webview->url().host() + " wants your location.");
                    break;
                case QWebEnginePage::Feature::MediaVideoCapture:
                    this->permissionDialog->exec(this->webview->url().host() + " is requesting camera access.");
                    break;
                case QWebEnginePage::Feature::MediaAudioVideoCapture:
                    this->permissionDialog->exec(this->webview->url().host() + " is requesting camera and mic access.");
                    break;
                case QWebEnginePage::Feature::DesktopVideoCapture:
                    this->permissionDialog->exec(this->webview->url().host() + " wants to share your screen.");
                    break;
                case QWebEnginePage::Feature::DesktopAudioVideoCapture:
                    this->permissionDialog->exec(this->webview->url().host() + " wants to share your screen.");
                    break;
                case QWebEnginePage::Feature::MouseLock:
                    this->webview->page()->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
                    break;
            }

            switch(this->permissionDialog->result()){
                case QDialog::Accepted:
                    this->webview->page()->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
                    this->permissions->at(feature) = true;
                    break;
                case QDialog::Rejected:
                    this->webview->page()->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
                    this->permissions->at(feature) = false;
                    break;
                default:
                    this->webview->page()->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionUnknown);
            }
            this->pageSettingsDialog->setPermissions(*this->permissions);
        } });

    this->connect(this->webview->page(), &QWebEnginePage::desktopMediaRequested, this, [=](const QWebEngineDesktopMediaRequest &request)
                  {
        //request.selectScreen(request.screensModel()->index(0));
        this->screenShareDialog = new ScreenShareDialog(this);
        this->screenShareDialog->exec(request);
        delete this->screenShareDialog; });

    // this->connect(this->profile, &QWebEngineProfile::downloadRequested, this, [=](QWebEngineDownloadRequest *download){
    //     download->accept();
    //     QString downloadPath = download->downloadDirectory() + "/" + download->downloadFileName();
    //     QFileInfo fileInfo(downloadPath);
    //     int progress = static_cast<int>(((download->totalBytes() - download->receivedBytes()) * 100) / download->totalBytes());
    //     this->downloadManager->addDownload(fileInfo.filePath(), progress);
    // });

    this->connect(this->profile, &QWebEngineProfile::downloadRequested, this, [=](QWebEngineDownloadRequest *download)
                  {
    qDebug() << "Download requested:" << download->downloadFileName();
    download->accept();
    QString downloadPath = download->downloadDirectory() + "/" + download->downloadFileName();
    QFileInfo fileInfo(downloadPath);
    qDebug() << "Full download path:" << fileInfo.filePath();

    this->downloadManager->addDownload(fileInfo.filePath(), 0);
    this->downloadManager->open();  // Make sure the download manager is visible

    this->connect(download, &QWebEngineDownloadRequest::receivedBytesChanged, this, [=]() {
        int progress = download->totalBytes() > 0 ? (download->receivedBytes() * 100) / download->totalBytes() : 0;
        qDebug() << "Download progress:" << fileInfo.filePath() << progress << "%";
        this->downloadManager->updateDownloadProgress(fileInfo.filePath(), progress);
    });

    this->connect(download, &QWebEngineDownloadRequest::isFinished, this, [=]() {
        qDebug() << "Download finished:" << fileInfo.filePath();
        this->downloadManager->updateDownloadProgress(fileInfo.filePath(), 100);
        this->downloadManager->saveDownloads();
    }); });

    this->connect(this->webview->page(), &QWebEnginePage::newWindowRequested, this, [=](QWebEngineNewWindowRequest &request)
                  {
        switch (request.destination()){
        case QWebEngineNewWindowRequest::DestinationType::InNewTab:
            emit this->newTabRequested(request.requestedUrl());
            break;
        case QWebEngineNewWindowRequest::DestinationType::InNewWindow:
            emit this->newWindowRequested(request.requestedUrl());
            break;
        } });

    this->connect(this->webview->page(), &QWebEnginePage::certificateError, this, [=](QWebEngineCertificateError certificateError)
                  {
        this->certificateErrorDialog->exec(certificateError);
        this->pageSettingsDialog->setSecure(false);

        switch(this->certificateErrorDialog->result()){
            case QDialog::Accepted:
                certificateError.acceptCertificate();
                break;
            case QDialog::Rejected:
                certificateError.rejectCertificate();
                break;
            default:
                certificateError.rejectCertificate();
        } });

    this->connect(this->tabTitleBar, &TabTitleBar::searchRequested, this, [=]()
                  {
                      emit this->searchRequested();
                      // this->searchDialog->open();
                  });

    // this->connect(this->searchDialog, &SearchDialog::accepted, this, [=](){
    //     this->webview->load(QUrl(this->searchDialog->getSearch()));
    // });

    this->connect(this->tabTitleBar, &TabTitleBar::copyLinkRequested, this, [=]()
                  { qApp->clipboard()->setText(this->webview->url().toString()); });

    this->connect(this->tabTitleBar, &TabTitleBar::previousPageRequested, this, [=]()
                  { this->webview->back(); });

    this->connect(this->tabTitleBar, &TabTitleBar::nextPageRequested, this, [=]()
                  { this->webview->forward(); });

    this->connect(this->tabTitleBar, &TabTitleBar::reloadRequested, this, [=]()
                  { this->webview->reload(); });

    this->connect(this->tabTitleBar, &TabTitleBar::splitTabLeftRequested, this, [=]()
                  { emit this->splitTabLeftRequested(); });

    this->connect(this->tabTitleBar, &TabTitleBar::splitTabRightRequested, this, [=]()
                  { emit this->splitTabRightRequested(); });

    this->connect(this->tabTitleBar, &TabTitleBar::splitTabFlipRequested, this, [=]()
                  { emit this->splitTabFlipRequested(); });

    this->connect(this->tabTitleBar, &TabTitleBar::closeTabRequested, this, [=]()
                  { emit this->closeTabRequested(); });

    this->connect(this->tabTitleBar, &TabTitleBar::siteSettingsRequested, this, [=]()
                  { this->pageSettingsDialog->open(); });

    this->layout->addWidget(this->tabTitleBar);
    this->layout->addWidget(this->progressIndicator);
    // this->layout->addWidget(this->pageSurface);

    this->devtoolsSplitter->addWidget(this->webview);
    this->layout->addWidget(this->devtoolsSplitter);
}

// adding a custom scroll bar which gets hidden when not in use
void Tab::initCustomScrollBar()
{
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

void Tab::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(palette().window());
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
}

void Tab::resizeEvent(QResizeEvent *event)
{
    // QPainterPath path;
    // path.addRoundedRect(rect(), 10, 10);
    // this->setMask(path.toFillPolygon().toPolygon());
}

void Tab::mousePressEvent(QMouseEvent *event)
{
    emit this->tabFocused();
}

void Tab::setTitleBarVisible(bool visible)
{
    this->tabTitleBar->setVisible(visible);
}

void Tab::requestSearchDialog()
{
    // this->searchDialog->open();
    emit this->searchRequested();
}

QString Tab::getTitle()
{
    return this->webview->title();
}

QIcon Tab::getIcon()
{
    return this->webview->icon();
}

void Tab::copyUrl()
{
    qApp->clipboard()->setText(this->webview->url().toString());
}

void Tab::requestNextPage()
{
    this->webview->forward();
}

void Tab::requestPreviousPage()
{
    this->webview->back();
}

void Tab::requestReload()
{
    this->webview->reload();
}

void Tab::openDevTools()
{
    if (!devtools)
    {
        this->devtools = new WebView();
        this->webview->page()->setDevToolsPage(this->devtools->page());
        this->devtoolsSplitter->insertWidget(1, this->devtools);

        this->connect(this->devtools->page(), &QWebEnginePage::windowCloseRequested, this, [=]()
                      { this->closeDevTools(); });
    }
}

void Tab::closeDevTools()
{
    delete this->devtools;
    this->devtools = nullptr;
}

void Tab::load(QUrl url)
{
    this->webview->load(url);
}

void Tab::showSiteSettings()
{
    this->pageSettingsDialog->open();
}

Tab::~Tab() = default;
