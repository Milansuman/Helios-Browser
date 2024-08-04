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

    this->connect(this->tabsApi, &TabsApi::loadUrl, this, [=](int group, int tab, QString url){
        emit this->loadUrl(group, tab, url);
    });

    this->fileApi = new FileApi();
    this->channel->registerObject("fs", this->fileApi);

    this->dialogApi = new DialogApi();
    this->channel->registerObject("dialog", this->dialogApi);

    this->ollamaApi = new OllamaApi();
    this->channel->registerObject("ollama", this->ollamaApi);

    this->channel->registerObject("misc", this);

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
                // window.ai = {
                //     generate: (model, prompt) => {
                //         return new Promise((resolve, reject) => {
                //             channel.objects.ollama.generate(model, prompt);

                //             channel.objects.ollama.responseGenerated.connect((response) => {
                //                 resolve(response);
                //             })
                //         });
                //     }
                // };

                window.currentTab = channel.objects.misc.tab;
                window.currentGroup = channel.objects.misc.group;
            });

            function inject(fn) {
                const script = document.createElement('script')
                script.text = `(${fn.toString()})();`
                document.documentElement.appendChild(script)
            }


            function run() {
                Object.defineProperty(navigator, 'userAgent', {
                    get: function () { return 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/127.0.0.0 Safari/537.36'; }
                });

                console.log(navigator.userAgent)

                window.ai =  {
                    canCreateGenericSession: async () => "readily",
                    canCreateTextSession: async () => "readily",
                    defaultTextSessionOptions: async () => {},
                    createTextSession: async () => {
                    function createStreamFromApi(message) {
                        let cumulativeContent = '';
                        const stream = new ReadableStream({
                        start(controller) {
                            fetch('http://localhost:11434/api/chat', {
                            method: 'POST',
                            headers: {
                                'Content-Type': 'application/json'
                            },
                            body: JSON.stringify({
                                model: OLLAMA_MODEL_NAME,
                                messages: [
                                {
                                    role: 'user',
                                    content: message
                                }
                                ]
                            })
                            })
                            .then(response => {
                            const reader = response.body.getReader();
                            const decoder = new TextDecoder();
                            
                            function push() {
                                reader.read().then(({ done, value }) => {
                                if (done) {
                                    controller.close();
                                    return;
                                }
                                
                                const text = decoder.decode(value, { stream: true });
                                const jsonObjects = text.trim().split('\n').map(line => JSON.parse(line));
                                for (const jsonObject of jsonObjects) {
                                    if (jsonObject.message && jsonObject.message.content) {
                                    cumulativeContent += jsonObject.message.content
                                    controller.enqueue(cumulativeContent);
                                    }
                                }
                                
                                push();
                                });
                            }
                            
                            push();
                            })
                            .catch(error => {
                            console.error(error);
                            controller.error(error);
                            });
                        }
                        });
                        
                        return stream;
                    }


                    return {
                        promptStreaming: createStreamFromApi
                    }
                    }
                }
            }


            inject(run)
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