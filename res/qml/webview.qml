import QtQuick 2.0
import QtWebEngine 1.1
import Qt5Compat.GraphicalEffects

WebEngineView{
    id: webview
    url: "https://savee.it"
    width: 0
    height: 0

    property int radius: 10

    layer.enabled: true
    layer.effect: OpacityMask{
        maskSource: Rectangle{
            anchors.fill: webview
            radius: 10
            color: "#ffffff"
        }
    }
}