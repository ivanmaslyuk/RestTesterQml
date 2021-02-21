import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15

import "."

ColumnLayout {
    spacing: 0

    required property string bodyText
    required property ListModel headersModel

    Rectangle {
        id: header
        implicitHeight: 40
        Layout.fillWidth: true

        RowLayout {
            spacing: 12
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16


            Text {
                text: qsTr("Response")
                font.pixelSize: 16
            }

            SwitchTabBar {
                id: tabBar
                tabsModel: [qsTr("Body"), qsTr("Headers")]
            }

            Item {
                RowLayout.fillWidth: true
            }

            Text {
                id: statusCode
                text: "200"
                color: "#808080"
                font.pixelSize: 12
            }

            Text {
                id: requestTime
                text: "56 ms"
                color: "#808080"
                font.pixelSize: 12
            }

            Text {
                id: responseSize
                text: "4.1 KB"
                color: "#808080"
                font.pixelSize: 12
            }
        }
    }

    StackLayout {
        id: stackLayout
        currentIndex: tabBar.currentIndex

        ScrollView {
            id: scrollView
            clip: true

            TextEdit {
                id: bodyTextEdit
                text: bodyText
                font.family: "Consolas"
                font.pixelSize: 12
                selectByMouse: true
                readOnly: true
                textMargin: 4
            }
        }

        Rectangle {
            id: headersTable

            ParamsTable {
                showCheckBox: false
                readOnly: true

                anchors.fill: parent
                anchors.topMargin: 12
                anchors.bottomMargin: 12
                anchors.leftMargin: 16
                anchors.rightMargin: 16

                listModel: headersModel
            }
        }
    }
}
