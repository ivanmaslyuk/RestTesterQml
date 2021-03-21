import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.15

import "components"

Window {
    id: window
    width: 750
    height: 600
    visible: true
    title: "REST Tester"

    Rectangle {
        height: 1
        color: "#EEEEEE"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        z: 100
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        AppMenu {
            id: menu
            Layout.fillWidth: true
        }

        SplitView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            handle: Rectangle {
                implicitWidth: 3
                color: "#EEEEEE"
            }

            CustomTreeView {
                id: treeView
                clip: true
                SplitView.minimumWidth: 100
                SplitView.maximumWidth: 250
            }

            RequestView {
                id: requestView
            }
        }
    }
}
