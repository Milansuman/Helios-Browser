import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Window {
    id: browserWindow
    flags: Qt.FramelessWindowHint
    width: 1080
    height: 800
    visible: true
    color: "transparent"
    property int bw: 5

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: {
            const p = Qt.point(mouseX, mouseY);
            const b = bw + 10; // Increase the corner size slightly
            if (p.x < b && p.y < b) return Qt.SizeFDiagCursor;
            if (p.x >= width - b && p.y >= height - b) return Qt.SizeFDiagCursor;
            if (p.x >= width - b && p.y < b) return Qt.SizeBDiagCursor;
            if (p.x < b && p.y >= height - b) return Qt.SizeBDiagCursor;
            if (p.x < b || p.x >= width - b) return Qt.SizeHorCursor;
            if (p.y < b || p.y >= height - b) return Qt.SizeVerCursor;
            return Qt.ArrowCursor
        }
        acceptedButtons: Qt.NoButton // don't handle actual events
    }

    DragHandler {
        id: resizeHandler
        grabPermissions: TapHandler.TakeOverForbidden
        target: null
        onActiveChanged: if (active) {
            const p = resizeHandler.centroid.position;
            const b = bw + 10; // Increase the corner size slightly
            let e = 0;
            if (p.x < b) { e |= Qt.LeftEdge }
            if (p.x >= width - b) { e |= Qt.RightEdge }
            if (p.y < b) { e |= Qt.TopEdge }
            if (p.y >= height - b) { e |= Qt.BottomEdge }
            browserWindow.startSystemResize(e);
        }
    }

    //maximise toggle function
    function toggleMaximized(){
        if(browserWindow.visibility === Window.Maximized){
            browserWindow.showNormal()
        }else{
            browserWindow.showMaximized()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        //Title bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: "#222222"
            topLeftRadius: 10.0
            topRightRadius: 10.0
            
            TapHandler {
                onTapped: if (tapCount === 2) toggleMaximized()
                gesturePolicy: TapHandler.DragThreshold
            }
            DragHandler {
                grabPermissions: TapHandler.CanTakeOverFromAnything
                onActiveChanged: if (active) { browserWindow.startSystemMove(); }
            }

            //TitleBar layout
            RowLayout {
                Layout.fillWidth: true

                //title bar buttons layout
                RowLayout {
                    Layout.margins: 10.0

                    //minimise
                    RoundButton{
                        background: Rectangle{
                            color: "#22CB3E"
                            radius: 100.0
                            width: 13
                            height: 13
                        }
                        onClicked: browserWindow.showMinimized()
                    }

                    //maximise
                    RoundButton{
                        background: Rectangle{
                            color: "#FFBC2F"
                            radius: 100.0
                            width: 13
                            height: 13
                        }
                        onClicked: toggleMaximized()
                    }
                    
                    //close
                    RoundButton{
                        background: Rectangle{
                            color: "#FF5D49"
                            radius: 100.0
                            width: 13
                            height: 13
                        }

                        onClicked: browserWindow.close()
                    }
                }
            }
        }

        //Content
        Rectangle{
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#222222"
            bottomLeftRadius: 10.0
            bottomRightRadius: 10.0
            Layout.topMargin: -1
        }
    }
}
