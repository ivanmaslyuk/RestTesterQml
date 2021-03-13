import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import "components"

Window {
    width: 600
    height: 600
    visible: true
    title: "REST Tester"
//    color: "black"

    Rectangle {
        height: 1
        color: "#EEEEEE"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        z: 100
    }

    SplitView {
        anchors.fill: parent

        handle: Rectangle {
            implicitWidth: 5
            color: "transparent"

            Rectangle {
                implicitWidth: 1
                color: "#EEEEEE"

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
            }
        }

        ColumnLayout {
            clip: true
            spacing: 12
            SplitView.minimumWidth: menu.implicitWidth
            SplitView.maximumWidth: 300

            AppMenu {
                id: menu
                Layout.fillWidth: true
            }

            Rectangle {
                color: "#EEEEEE"
                implicitHeight: 1

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.leftMargin: 16
                Layout.rightMargin: 16
            }

            CustomTreeView {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        RequestView {

        }

    }
}
