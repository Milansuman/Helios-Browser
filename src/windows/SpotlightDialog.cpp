#include "SpotlightDialog.h"

#include <QPainterPath>
#include <QPainter>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QList>
#include <QUrl>

SpotlightDialog::SpotlightDialog(QWidget *parent): QDialog(parent){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setFixedSize(parent->size());
    this->layout = new QVBoxLayout(this);

    this->webview = new WebView();
    this->webview->page()->setBackgroundColor(QColor(0, 0, 0, 0));

    this->webview->load(QUrl("https://jovit-mathew236.github.io/project-web-spotlight/"));

    this->channel = new QWebChannel(this->webview->page());
    this->webview->page()->setWebChannel(this->channel);

    this->tabsApi = new TabsApi();
    this->channel->registerObject("tabs", this->tabsApi);

    this->connect(this->tabsApi, &TabsApi::splitTabRequested, this, [=](QUrl url){
        emit this->splitTabRequested(url);
    });

    this->connect(this->tabsApi, &TabsApi::splitTabHomeRequested, this, [=](){
        emit this->splitTabHomeRequested();
    });

    this->connect(this->tabsApi, &TabsApi::newTabRequested, this, [=](QUrl url){
        emit this->newTabRequested(url);
    });

    this->connect(this->tabsApi, &TabsApi::splitTabFlipRequested, this, [=](){
        emit this->splitTabFlipRequested();
    });

    this->connect(this->tabsApi, &TabsApi::addTabsRequested, this, [=](QList<QList<QUrl>> tabsList){
        emit this->addTabsRequested(tabsList);
    });

    this->fileApi = new FileApi();
    this->channel->registerObject("fs", this->fileApi);

    this->dialogApi = new DialogApi();
    this->channel->registerObject("dialog", this->dialogApi);

    this->ollamaApi = new OllamaApi();
    this->channel->registerObject("ollama", this->ollamaApi);

    this->connect(this->dialogApi, &DialogApi::closeDialogRequested, this, [=](){
        this->accept();
    });

    QWebEngineScript script;
    script.setName("WebChannelScript");
    script.setSourceCode(R"(
        var script = document.createElement('script');
        script.src = 'qrc:///qtwebchannel/qwebchannel.js';

        script.onload = function() {
            new QWebChannel(qt.webChannelTransport, function(channel) {
                window.tabs = channel.objects.tabs;
                window.fs = channel.objects.fs;
                window.dialog = channel.objects.dialog;
                window.ai = {
                    generate: (model, prompt) => {
                        return new Promise((resolve, reject) => {
                            channel.objects.ollama.generate(model, prompt);

                            channel.objects.ollama.responseGenerated.connect((response) => {
                                resolve(response);
                            })
                        });
                    }
                };
            });
        };
        document.getElementsByTagName('head')[0].appendChild(script);
    )");

    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setWorldId(QWebEngineScript::MainWorld);
    script.setRunsOnSubFrames(true);

    this->webview->page()->scripts().insert(script);

    this->layout->addWidget(this->webview);
}

void SpotlightDialog::open(int pos, int group){
    this->pos = pos;
    this->group = group;
    QDialog::open();
}

SpotlightDialog::~SpotlightDialog() = default;