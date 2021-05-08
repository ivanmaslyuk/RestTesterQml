import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import "."

Rectangle {
    color: theme.primaryBackground

    Rectangle {
        id: noRequestSelectedView
        visible: !app.requestSelected
        anchors.fill: parent
        color: "transparent"

        Text {
            anchors.centerIn: parent
            color: theme.textColor
            text: qsTr("Выберитре запрос из списка слева.")
        }
    }

    SplitView {
        id: control
        visible: app.requestSelected
        anchors.fill: parent

        handle: Rectangle {
            color: "transparent"
            implicitWidth: 10

            Rectangle {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: 1
                color: theme.lineColor
            }
        }

        signal needMoreSpace(int addWidth)

        SplitView {
            id: requestPane
            orientation: Qt.Vertical
            SplitView.minimumWidth: 420
            SplitView.fillWidth: true
            clip: true

            handle: Rectangle {
                color: "transparent"
                implicitHeight: 10

                Rectangle {
                    height: 1
                    color: theme.lineColor
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
            }

            RequestInfoView {
                id: requestInfoView
                SplitView.minimumHeight: implicitHeight
                SplitView.maximumHeight: control.width - 40
            }

            ResponseWrapper {
                SplitView.minimumHeight: 40
                SplitView.fillHeight: true
            }
        }

        DocumentationView {
            id: documentationPane
            visible: requestInfoView.documentationShown
            SplitView.minimumWidth: 220
        }

        Connections {
            target: documentationPane
            function onVisibleChanged() {
                if (documentationPane.visible && control.width < 700) {
                    control.needMoreSpace(300)
                }
                documentationPane.SplitView.preferredWidth = 300
            }
        }
    }
}
