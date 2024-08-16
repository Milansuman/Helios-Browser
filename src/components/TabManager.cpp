#include "TabManager.h"

#include <QWebEngineSettings>
#include <QWebEngineCookieStore>
#include <QWebEngineDownloadRequest>
#include <QShortcut>
#include <QApplication>
#include <QTimer>

TabManager::TabManager(QWidget *parent) : QStackedWidget(parent), currentGroup(0)
{
    this->setMouseTracking(true);
    this->profile = new QWebEngineProfile("project-web");
    this->profile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
    this->profile->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    this->profile->settings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, true);
    this->profile->settings()->setAttribute(QWebEngineSettings::WebGLEnabled, true);
    this->profile->settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);
    this->profile->settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);

    this->groups.push_back(new TabGroup(profile));

    this->downloadManager = new DownloadManager(this);

    this->downloadRequests = QList<QWebEngineDownloadRequest*>();
    this->history = QList<History>();

    this->connect(this->profile, &QWebEngineProfile::downloadRequested, this, [=](QWebEngineDownloadRequest *request){
        request->accept();
        this->downloadRequests.append(request);
        this->downloadManager->addDownloadItem(request);
        this->windowShowDownloads();

        emit this->downloadsChanged(this->downloadRequests.size());

        this->connect(request, &QWebEngineDownloadRequest::receivedBytesChanged, this, [=](){
            this->downloadManager->updateDownloadItem(request);
        });

        this->connect(request, &QWebEngineDownloadRequest::isFinishedChanged, this, [=](){
            if(request->isFinished()) this->downloadManager->finishDownloadItem(request);
        });
    });

    QShortcut *openDevToolsAction = new QShortcut(this);
    openDevToolsAction->setKeys(QList<QKeySequence>({Qt::Key_F12, Qt::CTRL | Qt::SHIFT | Qt::Key_I}));

    this->connect(openDevToolsAction, &QShortcut::activated, this, [=](){ 
        this->getCurrentGroup()->openDevTools(); 
    });

    this->groupSelectorDialog = new GroupSelectorDialog(this);
    this->groupSelectorDialog->addGroup(new GroupIcons(this->groups.at(0)));

    this->connect(this->groupSelectorDialog, &GroupSelectorDialog::changeGroupRequested, this, [=](int pos){
        this->currentGroup = pos;
        this->setCurrentIndex(pos);

        if(this->getCurrentGroup()->getTabs().size() == 1){
            emit this->displayTitleBarOnWindowRequested();
            emit this->titleChanged(this->getCurrentGroup()->getTab(0)->getTitle());
        }else{
            emit this->hideTitleBarOnWindowRequested();
        } 
    });

    this->connect(this->groupSelectorDialog, &GroupSelectorDialog::closeGroupRequested, this, [=](int pos){
        if(this->currentGroup == pos && this->currentGroup != 0){
            this->currentGroup = pos-1;
            this->setCurrentIndex(pos-1);
        }

        if(this->getCurrentGroup()->getTabs().size() == 1){
            emit this->displayTitleBarOnWindowRequested();
            emit this->titleChanged(this->getCurrentGroup()->getTab(0)->getTitle());
        }else{
            emit this->hideTitleBarOnWindowRequested();
        }
        this->closeGroup(pos); 
    });

    this->connect(this->groups.at(0), &TabGroup::searchRequested, this, [=](int pos){ 
        emit this->searchRequested(pos, 0); 
    });

    this->connect(this->groups.at(0), &TabGroup::tabsChanged, this, [=](){
        if(this->groups.at(0)->getTabs().size() == 1){
            emit this->displayTitleBarOnWindowRequested();
        }else{
            emit this->hideTitleBarOnWindowRequested();
        } 
    });

    this->connect(this->groups.at(0), &TabGroup::windowTitleChanged, this, [=](QString title){ 
        emit this->titleChanged(title); 
    });

    this->connect(this->groups.at(0), &TabGroup::newTabRequested, this, [=](QUrl url){
        this->addGroup();
        this->getCurrentGroup()->getTab(0)->load(url); 
    });

    this->connect(this->groups.at(0), &TabGroup::newWindowRequested, this, [=](QUrl url){ 
        emit this->newWindowRequested(url); 
    });

    this->connect(this->groups.at(0), &TabGroup::navigationRequested, this, [=](QString title, QUrl url){
        this->history.append({
            title,
            url,
            QDateTime::currentDateTime()
        });
    });

    this->addWidget(this->groups.at(0));
}

TabGroup *TabManager::getGroup(int pos)
{
    return this->groups.at(pos);
}

TabGroup *TabManager::getCurrentGroup()
{
    return this->groups.at(this->currentGroup);
}

void TabManager::addGroup()
{
    this->groups.push_back(new TabGroup(profile));
    int pos = this->groups.size() - 1;
    this->groupSelectorDialog->addGroup(new GroupIcons(this->groups.at(pos)));

    this->connect(this->groups.at(pos), &TabGroup::searchRequested, this, [=](int i){ 
        emit this->searchRequested(i, pos); 
    });

    this->connect(this->groups.at(pos), &TabGroup::searchRequested, this, [=](int i){ 
        emit this->searchRequested(i, pos); 
    });

    this->connect(this->groups.at(pos), &TabGroup::tabsChanged, this, [=](){
        if(this->groups.at(pos)->getTabs().size() == 1){
            emit this->displayTitleBarOnWindowRequested();
        }else{
            emit this->hideTitleBarOnWindowRequested();
        } 
    });

    this->connect(this->groups.at(pos), &TabGroup::windowTitleChanged, this, [=](QString title){ 
        emit this->titleChanged(title); 
    });

    this->connect(this->groups.at(pos), &TabGroup::newTabRequested, this, [=](QUrl url){
        this->addGroup();
        this->getCurrentGroup()->getTab(0)->load(url); 
    });

    this->connect(this->groups.at(pos), &TabGroup::newWindowRequested, this, [=](QUrl url){ 
        emit this->newWindowRequested(url); 
    });

    this->connect(this->groups.at(pos), &TabGroup::navigationRequested, this, [=](QString title, QUrl url){
        this->history.append({
            title,
            url,
            QDateTime::currentDateTime()
        });
    });

    this->currentGroup = pos;
    this->addWidget(this->groups.at(pos));
    this->setCurrentIndex(pos);
    emit this->displayTitleBarOnWindowRequested();
    emit this->titleChanged(this->getCurrentGroup()->getTab(0)->getTitle());
}

void TabManager::addGroup(QUrl url){
    this->addGroup();
    this->getCurrentGroup()->getTab(0)->load(url);
}

void TabManager::closeGroup(int pos)
{
    delete this->groups.at(pos);
    this->groups.erase(this->groups.begin() + pos);

    if (this->groups.size() == 0){
        qApp->quit();
    }
}

void TabManager::setInitialUrl(QUrl url)
{
    this->getCurrentGroup()->getTab(0)->load(url);
}

void TabManager::windowSplitLeft()
{
    this->getCurrentGroup()->splitLeft(0);
}

void TabManager::windowSplitRight()
{
    this->getCurrentGroup()->splitRight(0);
}

void TabManager::windowCopyLink()
{
    this->getCurrentGroup()->getTab(0)->copyUrl();
}

void TabManager::windowSearch()
{
    this->getCurrentGroup()->getTab(0)->requestSearchDialog();
}

void TabManager::windowNextPage()
{
    this->getCurrentGroup()->getTab(0)->requestNextPage();
}

void TabManager::windowPreviousPage()
{
    this->getCurrentGroup()->getTab(0)->requestPreviousPage();
}

void TabManager::windowReload()
{
    this->getCurrentGroup()->getTab(0)->requestReload();
}

void TabManager::windowShowGroups()
{
    this->groupSelectorDialog->open();
}

void TabManager::windowShowSiteSettings()
{
    this->getCurrentGroup()->getTab(0)->showSiteSettings();
}

void TabManager::windowLoadBulk(QList<QList<QUrl>> urlsList)
{
    qDebug() << urlsList;
    for (QList<QUrl> group : urlsList){
        this->addGroup();
        for(int i=0; i<group.size()-1; i++){
            this->windowSplitRight();
        }

        for (int i = 0; i < this->getCurrentGroup()->getTabs().size(); i++){
            this->getCurrentGroup()->getTab(i)->load(group.at(i));
        }
    }
}

void TabManager::windowShowDownloads(){
    this->downloadManager->open();
}

void TabManager::windowLoadTab(int group, int tab, QString url){
    this->getGroup(group)->getTab(tab)->load(QUrl::fromUserInput(url));
}

TabManager::~TabManager()
{
    for (TabGroup *group : this->groups){
        delete group;
    }
    this->groups.clear();
}