import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import "components"

Window {
    width: 800
    height: 600
    visible: true
    title: "REST Tester"

    Rectangle {
        height: 1
        color: "#EEEEEE"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    SplitView {
        anchors.fill: parent

        CustomTreeView {

        }

        RequestView {

        }

    }
}
