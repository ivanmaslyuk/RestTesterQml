import QtQuick 2.15
import QtQuick.Window 2.15

import "components"

Window {
    width: 400
    height: 600
    visible: true
    title: "REST Tester"

    MainWidget {

        anchors.top: parent.top
        anchors.topMargin: 12
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 12
    }
}
