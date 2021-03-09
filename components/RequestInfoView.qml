import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "."

ColumnLayout  {
    id: rootLayout
    spacing: 12

    property bool documentationShown: false
    property bool paramsVisible: true

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
            text: app.activeRequest.edited ?
                      app.activeRequest.name + " *" : app.activeRequest.name
            font.pixelSize: 16
        }

        Rectangle { Layout.fillWidth: true }

        LinkButton {
            text: documentationShown ? qsTr("Скрыть документацию") : qsTr("Документация")
            Layout.alignment: Qt.AlignVCenter

            onPressed: {
                documentationShown = !documentationShown
            }
        }
    }

    RowLayout {
        spacing: 12
        Layout.preferredHeight: 40
        Layout.maximumHeight: 40
        Layout.minimumHeight: 40
        Layout.leftMargin: 16
        Layout.rightMargin: 16

        Rectangle {
            id: inputGroup
            border.color: "#EEEEEE"
            border.width: 1
            radius: 4
            Layout.fillWidth: true
            Layout.fillHeight: true

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

                    model: [
                        { text: "GET", value: "GET" },
                        { text: "POST", value: "POST" },
                        { text: "PUT", value: "PUT" },
                        { text: "PATCH", value: "PATCH" },
                        { text: "DELETE", value: "DELETE" },
                    ]

                    onCurrentValueChanged: {
                        app.activeRequest.method = methodSelect.currentValue
                    }

                    Connections {
                        target: app
                        function onActiveRequestChanged(request) {
                            methodSelect.currentIndex = methodSelect.indexOfValue(request.method)
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 24
                    color: "#EEEEEE"

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

                    background: Rectangle { }

                    onTextChanged: {
                        app.activeRequest.url = urlField.text
                    }

                    Connections {
                        target: app
                        function onActiveRequestChanged(request) {
                            urlField.text = request.url
                        }
                    }
                }

            }
        }

        CustomButton {
            id: sendButton
            Layout.fillHeight: true

            background: Rectangle {
                color: "#48B146"
                radius: 4
            }

            contentItem: Text {
                font.pixelSize: 16
                font.bold: true
                text: "ОТПРАВИТЬ"
                color: "white"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    RowLayout {
        spacing: 12
        Layout.fillHeight: false

        SwitchTabBar {
            id: tabBar
            Layout.leftMargin: 16
            tabsModel: [qsTr("Запрос"), qsTr("Тело"), qsTr("Заголовки"), qsTr("Тесты")]
        }

        StyledComboBox {
            id: contentTypeComboBox
            visible: tabBar.currentIndex == 1
            Layout.fillHeight: true
            textRole: "text"
            valueRole: "value"
            model: [
                { text: qsTr("Без тела"), value: "" },
                { text: "URL-encoded", value: "application/x-www-form-urlencoded" },
                { text: "Form Data", value: "multipart/form-data" },
                { text: "JSON", value: "application/json" },
                { text: "Plain Text", value: "text/plain" },
                { text: "HTML", value: "text/html" },
            ]

            onCurrentValueChanged: {
                app.activeRequest.contentType = contentTypeComboBox.currentValue
            }

            Connections {
                target: app
                function onActiveRequestChanged(request) {
                    contentTypeComboBox.currentIndex = contentTypeComboBox.indexOfValue(request.contentType)
                }
            }
        }

        Rectangle { Layout.fillWidth: true }

        LinkButton {
            text: rootLayout.paramsVisible ? "Скрыть" : "Показать"
            onPressed: {
                rootLayout.paramsVisible = !rootLayout.paramsVisible
            }
            Layout.rightMargin: 16
        }
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
        Layout.bottomMargin: 12
        Layout.minimumHeight: 60

        ParamsTable {
            id: queryParamsTable

            Connections {
                target: app
                function onActiveRequestChanged(request) {
                    queryParamsTable.model = request.queryParamsModel
                }
            }
        }

        StackLayout {
            id: bodyEditArea

            currentIndex: {
                const index = contentTypeComboBox.currentIndex
                if (index === 0)
                    return 0

                if (index >= 1 && index <= 2)
                    return 1

                return 2
            }

            Rectangle {
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: Qt.AlignHCenter
                    font.pixelSize: 14
                    text: qsTr("У этого запроса нет тела.")
                }
            }

            ParamsTable {
                id: bodyParamsTable

                Connections {
                    target: app
                    function onActiveRequestChanged(request) {
                        bodyParamsTable.model = request.dataParamsModel
                    }
                }
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
                    font.pixelSize: 12
                    font.family: "Consolas"
                    background: Rectangle {
                        border.color: "#EEEEEE"
                        border.width: 1
                        radius: 4
                    }

                    Connections {
                        target: app
                        function onActiveRequestChanged (request) {
                            rawDataTextArea.text = request.rawData
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar { }
            }
        }

        ParamsTable {
            id: headersTable

            Connections {
                target: app
                function onActiveRequestChanged(request) {
                    headersTable.model = request.headersModel
                }
            }
        }
    }

}
