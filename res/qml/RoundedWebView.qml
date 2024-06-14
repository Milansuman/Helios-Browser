import QtQuick
import QtWebEngine

Rectangle {
    id: backgroundRect
    anchors.fill: parent
    bottomLeftRadius: 10
    bottomRightRadius: 10
    color: "transparent"
    clip: true

    WebEngineView {
        id: webview
        objectName: webview
        anchors.fill: parent
    }
}