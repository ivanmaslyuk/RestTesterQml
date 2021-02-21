import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15

import "."

ColumnLayout {
//    implicitHeight: header.implicitHeight + stackLayout.implicitHeight
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

            SwitchTabView {
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

        Flickable {
            contentHeight: bodyTextEdit.implicitHeight
            contentWidth: bodyTextEdit.implicitWidth
            clip: true

            ScrollBar.vertical: ScrollBar {
                policy: (bodyTextEdit.height > parent.height) ?
                            ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            }
            ScrollBar.horizontal: ScrollBar {
                policy: (bodyTextEdit.width > parent.width) ?
                            ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            }

            TextEdit {
                id: bodyTextEdit
                text: "{\n  \"key\": \":value\",\n  {\n    \"key2\": \"value2 is very long\"\n  }\n}"
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

                listModel: ListModel {
                    ListElement {
                        name: "Status"
                        value: "200"
                        isChecked: true
                    }
                    ListElement {
                        name: "Content-Length"
                        value: "22"
                        isChecked: false
                    }
                    ListElement {
                        name: "Content-Type"
                        value: "application/json"
                        isChecked: true
                    }
                }
            }
        }
    }
}
