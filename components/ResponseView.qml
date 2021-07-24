import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import "."

ColumnLayout {
    id: control
    spacing: 0

    property var response: null

    Rectangle {
        id: header
        implicitHeight: 40
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        radius: 4
        color: theme.secondaryBackground

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
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        Layout.topMargin: 12

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
                width: stackLayout.width
                readOnly: true
                textMargin: 0
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }

        Rectangle {
            id: headers
            color: "transparent"

            ParamsTable {
                id: headersTable
                readOnly: true
                appendEmptyRow: false
                showCheckBox: false
                model: response ? response.headersModel : null

                anchors.fill: parent
//                anchors.topMargin: 12
//                anchors.bottomMargin: 12
//                anchors.leftMargin: 16
//                anchors.rightMargin: 16
            }
        }

        Flickable {
            id: testResultsList
            clip: true
            contentHeight: testResultsContent.height
            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.vertical: ScrollBar { }

            Column {
                id: testResultsContent

                Repeater {
                    model: ListModel { id: testResultsModel }

                    Rectangle {
                        height: 30
                        width: testResultsList.width
                        color: hovered ? theme.treeViewHighlight : "transparent"

                        property bool hovered: false

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 16
                            anchors.rightMargin: 16

                            Text {
                                color: theme.textColor
                                text: model.testName
                                font.pixelSize: 14
                            }

                            Rectangle { Layout.fillWidth: true }

                            Text {
                                text: model.succeeded ? "Пройден" : "Провален"
                                color: model.succeeded ? "green" : "red"
                                font.pixelSize: 14
                            }
                        }

                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            height: 1
                            color: theme.lineColor
                            opacity: 0.5
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: parent.hovered = true
                            onExited: parent.hovered = false
                        }
                    }
                }
            }
        }

    }

    Connections {
        target: app.httpClient
        function onResponseUpdated(response) {
            control.response = response

            // Update test results model
            var results = testRunner.runTests(app.activeRequest, response)
            testResultsModel.clear()
            for (var result of results) {
                testResultsModel.append(result)
            }
        }
    }
}
