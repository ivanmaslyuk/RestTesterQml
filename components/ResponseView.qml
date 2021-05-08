import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15

import "."

ColumnLayout {
    id: control
    spacing: 0

    property var response: null

    Rectangle {
        id: header
        implicitHeight: 40
        Layout.fillWidth: true
        color: "transparent"

        RowLayout {
            spacing: 12
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16

            Text {
                text: qsTr("Ответ")
                color: theme.textColor
                font.pixelSize: 16
            }

            SwitchTabBar {
                id: tabBar
                tabsModel: [qsTr("Тело"), qsTr("Заголовки"), qsTr("Результаты тестов")]
            }

            Item {
                RowLayout.fillWidth: true
            }

            Text {
                id: statusCode
                text: response ? response.status : ""
                color: "#808080"
                font.pixelSize: 12
            }

            Text {
                id: requestTime
                text: response ? response.time + " ms" : ""
                color: "#808080"
                font.pixelSize: 12
            }

            Text {
                id: responseSize
                text: response ? response.size + " B" : ""
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
                text: response ? response.body : ""
                font.family: "Consolas"
                font.pixelSize: 12
                selectByMouse: true
                color: theme.textColor
                readOnly: true
                textMargin: 4
            }
        }

        Rectangle {
            color: "transparent"

            ParamsTable {
                id: headersTable
                readOnly: true
                appendEmptyRow: false
                showCheckBox: false
                model: response ? response.headersModel : null

                anchors.fill: parent
                anchors.topMargin: 12
                anchors.bottomMargin: 12
                anchors.leftMargin: 16
                anchors.rightMargin: 16
            }
        }
    }

    Connections {
        target: app.httpClient
        function onResponseUpdated(response) {
            control.response = response
        }
    }
}
