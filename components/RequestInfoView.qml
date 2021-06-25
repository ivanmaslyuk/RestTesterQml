import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQml 2.15

import "."

ColumnLayout  {
    id: rootLayout
    spacing: 12

    property bool documentationShown: false
    property bool paramsVisible: true
    property var request: app.activeRequest
    property bool updatingRequest: true

    Connections {
        target: app
        function onActiveRequestChanged(request) {
            updatingRequest = true
            rootLayout.request = request
            updatingRequest = false
        }
    }

    onHeightChanged: {
        paramsVisible = true
    }

    RowLayout {
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        Layout.topMargin: 12
        Layout.fillWidth: true

        Text {
            id: requestName
            color: theme.textColor
            text: request.name
            font.pixelSize: 16
        }

        Rectangle {
            height: 6
            width: 6
            color: theme.accentColor
            radius: 3
            visible: request.edited
        }

        Rectangle { Layout.fillWidth: true }

        LinkButton {
            visible: request.edited
            text: qsTr("Сохранить")
            color: theme.textColor
            onClicked: app.saveCurrentRequest()
        }

        LinkButton {
            text: documentationShown ? qsTr("Скрыть документацию")
                                     : qsTr("Документация")
            Layout.alignment: Qt.AlignVCenter
            color: theme.textColor

            onPressed: {
                documentationShown = !documentationShown
            }
        }
    }

    RowLayout {
        id: urlFieldArea
        spacing: 12
        Layout.preferredHeight: 40
        Layout.maximumHeight: 40
        Layout.minimumHeight: 40
        Layout.leftMargin: 16
        Layout.rightMargin: 16

        Rectangle {
            id: inputGroup
            border.color: theme.lineColor
            border.width: 1
            border.pixelAligned: true
            radius: 4
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"

            RowLayout {
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                anchors.topMargin: 1
                anchors.bottomMargin: 1
                spacing: 12

                StyledComboBox {
                    id: methodSelect
                    font.pixelSize: 16
                    Layout.fillHeight: true
                    sidePadding: 0
                    hideBackground: true
                    textRole: "text"
                    valueRole: "value"
                    currentIndex: methodSelect.indexOfValue(request.method)

                    model: [
                        { text: "GET", value: "GET" },
                        { text: "POST", value: "POST" },
                        { text: "PUT", value: "PUT" },
                        { text: "PATCH", value: "PATCH" },
                        { text: "DELETE", value: "DELETE" },
                    ]

                    Binding {
                        target: request
                        property: "method"
                        value: methodSelect.currentValue
                        when: !updatingRequest
                        restoreMode: Binding.RestoreBinding
                    }
                }

                Rectangle {
                    width: 1
                    height: 24
                    color: theme.lineColor

                    Layout.bottomMargin: 8
                    Layout.topMargin: 8
                }

                TextField {
                    id: urlField
                    placeholderText: "Введите URL здесь"
                    placeholderTextColor: "#555555"
                    Layout.fillWidth: true
                    font.family: "Roboto"
                    font.pixelSize: 16
                    selectByMouse: true
                    leftPadding: 0
                    rightPadding: 0
                    text: request.url
                    color: theme.textColor

                    background: Rectangle { color: "transparent" }

                    Binding {
                        target: request
                        property: "url"
                        value: urlField.text
                        when: !updatingRequest
                        restoreMode: Binding.RestoreBinding

                    }
                }

            }
        }

        SendButton {
            function onClicked() {
                app.httpClient.makeRequest(app.activeRequest)
            }
        }
    }

    RowLayout {
        spacing: 12
        Layout.fillHeight: false

        SwitchTabBar {
            id: tabBar
            Layout.leftMargin: 16
            tabsModel: [qsTr("Запрос"), qsTr("Тело"), qsTr("Заголовки"), qsTr("Тесты"), qsTr("Документация")]
        }

        StyledComboBox {
            id: contentTypeComboBox
            visible: tabBar.currentIndex == 1
            Layout.fillHeight: true
            textRole: "text"
            valueRole: "value"
            currentIndex: indexOfValue(request.contentType)
            model: [
                { text: qsTr("Без тела"), value: "" },
                { text: "URL-encoded", value: "application/x-www-form-urlencoded" },
                { text: "JSON", value: "application/json" },
                { text: "Plain Text", value: "text/plain" },
                { text: "HTML", value: "text/html" },
            ]

            Binding {
                target: request
                property: "contentType"
                value: contentTypeComboBox.currentValue
                when: !updatingRequest
                restoreMode: Binding.RestoreBinding
            }
        }

        Rectangle { Layout.fillWidth: true }

//        LinkButton {
//            text: rootLayout.paramsVisible ? "Скрыть" : "Показать"
//            onPressed: {
//                rootLayout.paramsVisible = !rootLayout.paramsVisible
//            }
//            Layout.rightMargin: 16
//        }
    }

    Rectangle {
        visible: !rootLayout.paramsVisible
        Layout.fillHeight: true
    }

    StackLayout {
        visible: rootLayout.paramsVisible
        currentIndex: tabBar.currentIndex
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        Layout.bottomMargin: 12 - 9  // 10 px SplitView handle at the bottom
        Layout.minimumHeight: 63

        ParamsTable {
            id: queryParamsTable
            model: request.queryParamsModel
        }

        StackLayout {
            id: bodyEditArea

            currentIndex: {
                const index = contentTypeComboBox.currentIndex
                if (index === 0)
                    return 0

                if (index === 1)
                    return 1

                return 2
            }

            Rectangle {
                color: "transparent"

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: Qt.AlignHCenter
                    font.pixelSize: 14
                    color: theme.textColor
                    text: qsTr("У этого запроса нет тела")
                }
            }

            ParamsTable {
                id: bodyParamsTable
                model: request.dataParamsModel
            }

            Flickable {
                id: flickable
                anchors.fill: parent
                boundsBehavior: Flickable.StopAtBounds
                maximumFlickVelocity: 500
                clip: true

                TextArea.flickable: TextArea {
                    id: rawDataTextArea
                    wrapMode: TextArea.Wrap
                    selectByMouse: true
                    placeholderText: qsTr("Начните печатать здесь...")
                    text: request.rawData
                    font.pixelSize: 12
                    font.family: "Consolas"
                    color: theme.textColor
                    background: Rectangle {
                        color: "transparent"
                        border.color: theme.lineColor
                        border.width: 1
                        radius: 4
                    }

                    Binding {
                        target: request
                        property: "rawData"
                        value: rawDataTextArea.text
                        when: !updatingRequest
                        restoreMode: Binding.RestoreBinding
                    }
                }

                ScrollBar.vertical: ScrollBar { }
            }
        }

        ParamsTable {
            id: headersTable
            model: request.headersModel
        }

        Flickable {
            id: testsArea
            boundsBehavior: Flickable.StopAtBounds
            maximumFlickVelocity: 500
            clip: true

            TextArea.flickable: TextArea {
                id: testsInput
                wrapMode: TextArea.Wrap
                selectByMouse: true
                placeholderText: qsTr("test('example test', function (request, response) {\n    assert(response.status === 200)\n})")
                text: request.tests
                font.pixelSize: 12
                font.family: "Consolas"
                color: theme.textColor
                background: Rectangle {
                    color: "transparent"
                    border.color: theme.lineColor
                    border.width: 1
                    radius: 4
                }

                Binding {
                    target: request
                    property: "tests"
                    value: testsInput.text
                    when: !updatingRequest
                    restoreMode: Binding.RestoreBinding
                }
            }

            ScrollBar.vertical: ScrollBar { }
        }

        Flickable {
            boundsBehavior: Flickable.StopAtBounds
            clip: true

            TextArea.flickable: TextArea {
//                background: Rectangle {color: "red"}
                id: documentationShow
                leftPadding: 2
                rightPadding: 2
                topPadding: 0
                bottomPadding: 0
                wrapMode: TextArea.Wrap
                selectByMouse: true
                placeholderText: qsTr("Нет документации")
                color: theme.textColor
                font.pixelSize: 12
                readOnly: true
                textFormat: TextArea.MarkdownText
                onLinkActivated: Qt.openUrlExternally(link)
                onLinkHovered: { }

                Connections {
                    target: app
                    function onActiveRequestChanged(request) {
                        documentationShow.text = request.documentation
                        documentationShow.update()
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {}
        }

    }

}
