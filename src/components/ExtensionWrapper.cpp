#include "ExtensionWrapper.h"
#include <QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

ExtensionWrapper::ExtensionWrapper(QUrl path, QWidget *parent): QQuickWidget(parent){
    this->setSource(QUrl::fromLocalFile(":/qml/webview.qml"));

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setClearColor(Qt::transparent);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->root = this->rootObject();
    this->initialize(path);
}

void ExtensionWrapper::initialize(QUrl path){
    QFile manifestFile(path.toLocalFile() + "/manifest.json");
    manifestFile.open(QIODevice::ReadOnly);
    QJsonDocument manifestJSON = QJsonDocument::fromJson(manifestFile.readAll());

    QJsonObject manifest = manifestJSON.object();

    this->name = manifest.value("name").toString();
    this->description = manifest.value("description").toString();
    this->version = manifest.value("version").toString();

    QJsonObject action = manifest.value("action").toObject();
    this->root->setProperty("url", QUrl("file:///" + path.toLocalFile() + "/" + action.value("default_popup").toString()));
}

// void ExtensionWrapper::paintEvent(QPaintEvent *event){
//     QPainter painter(this);

//     painter.setBrush(QBrush(QColor(0, 0, 0, 0)));
//     painter.setPen(Qt::NoPen);
//     painter.drawRect(rect());

//     QQuickWidget::paintEvent(event);
// }

void ExtensionWrapper::resizeEvent(QResizeEvent *event){
    this->root->setSize(this->size());
}

ExtensionWrapper::~ExtensionWrapper() = default;