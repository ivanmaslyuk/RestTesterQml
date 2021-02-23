import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "."

ColumnLayout  {
    id: rootLayout
    spacing: 12

    property var name: "Create User"
    property var url: "http://example.com/?q=red+cars"
    property bool documentationShown: false

    RowLayout {
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        Layout.topMargin: 12
        Layout.fillWidth: true

        Text {
            text: name
            font.family: "Roboto"
            font.pixelSize: 16
        }

        Rectangle { Layout.fillWidth: true }

        LinkButton {
            text: documentationShown ? qsTr("Hide docs") : qsTr("Docs")
            Layout.alignment: Qt.AlignVCenter

            onClicked: {
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

                ComboBox {
                    id: methodComboBox
                    model: ["GET", "POST", "PUT", "PATCH", "DELETE"]
                    font.pixelSize: 16
                    font.family: "Roboto"

                    Layout.fillHeight: true

                    delegate: ItemDelegate {
                        width: 120
                        contentItem: Text {
                            text: modelData
                            color: "#555555"
                            font: methodComboBox.font
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                        highlighted: methodComboBox.highlightedIndex === index
                    }

                    popup: Popup {
                        y: methodComboBox.height - 1
                        width: 120
                        implicitHeight: contentItem.implicitHeight
                        padding: 1

                        contentItem: ListView {
                            boundsBehavior: Flickable.StopAtBounds
                            clip: true
                            implicitHeight: contentHeight
                            model: methodComboBox.popup.visible ? methodComboBox.delegateModel : null
                            currentIndex: methodComboBox.highlightedIndex

                            ScrollIndicator.vertical: ScrollIndicator { }
                        }

                        background: Rectangle {
                            border.color: "#EEEEEE"
                            radius: 4
                        }
                    }

                    contentItem: Text {
                        text: methodComboBox.currentText

                        verticalAlignment: Text.AlignVCenter
                        font: methodComboBox.font
                        color: "#555555"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    indicator: Canvas {
                        id: canvas
                        x: parent.contentItem.width
                        y: methodComboBox.topPadding + (methodComboBox.availableHeight - height) / 2
                        width: 8
                        height: 6
                        contextType: "2d"

                        Connections {
                            target: methodComboBox
                            function onPressedChanged() { canvas.requestPaint(); }
                        }

                        onPaint: {
                            context.reset();
                            context.moveTo(0, 0);
                            context.lineTo(width, 0);
                            context.lineTo(width / 2, height);
                            context.closePath();
                            context.fillStyle = "#555555";
                            context.fill();
                        }
                    }

                    background: Rectangle {
                        implicitWidth: methodComboBox.contentItem.implicitWidth + methodComboBox.indicator.width + 6
                        height: 38
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
                    text: url
                    Layout.fillWidth: true
                    font.family: "Roboto"
                    font.pixelSize: 16
                    selectByMouse: true
                    leftPadding: 0
                    rightPadding: 0

                    background: Rectangle { }

                    Text {
                        text: "Enter URL Here"
                        font: parent.font
                        color: "#555555"
                        visible: !parent.text
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

            }
        }

        Button {
            id: sendButton
            Layout.preferredWidth: 70
            Layout.fillHeight: true

            background: Rectangle {
                color: "#48B146"
                radius: 4
            }

            contentItem: Text {
                font.pixelSize: 16
                font.bold: true
                font.family: "Roboto"
                text: "SEND"
                color: "white"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    RowLayout {
        spacing: 0
        Layout.fillHeight: false

        SwitchTabBar {
            id: tabBar
            Layout.leftMargin: 16
            tabsModel: ["Query", "Body", "Headers"]
        }

        Rectangle { Layout.fillWidth: true }

        StyledComboBox {
            id: contentTypeComboBox
            visible: tabBar.currentIndex == 1
            Layout.fillHeight: true
            Layout.rightMargin: 16
            model: ["No body", "form-data", "x-www-form-urlencoded", "json", "raw"]
        }
    }

    StackLayout {
        currentIndex: tabBar.currentIndex
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        Layout.bottomMargin: 12
        Layout.minimumHeight: 60

        ParamsTable {
            showCheckBox: true
            listModel: ListModel {
                ListElement {
                    name: "q"
                    value: "red cars"
                    isChecked: true
                }
                ListElement {
                    name: "offset"
                    value: "22"
                    isChecked: false
                }
                ListElement {
                    name: "count"
                    value: "3"
                    isChecked: true
                }
                ListElement {
                    name: "q"
                    value: "red cars"
                    isChecked: true
                }
                ListElement {
                    name: "offset"
                    value: "22"
                    isChecked: false
                }
                ListElement {
                    name: "count"
                    value: "3"
                    isChecked: true
                }
                ListElement {
                    name: "q"
                    value: "red cars"
                    isChecked: true
                }
                ListElement {
                    name: "offset"
                    value: "22"
                    isChecked: false
                }
                ListElement {
                    name: "count"
                    value: "3"
                    isChecked: true
                }
            }
        }

        StackLayout {
            currentIndex: {
                const index = contentTypeComboBox.currentIndex
                if (index === 0)
                    return 0

                if (index > 0 && index < 3)
                    return 1

                return 2
            }

            Rectangle {
                Text {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 16
                    horizontalAlignment: Qt.AlignHCenter
                    font.pixelSize: 14
                    text: "This request has no body."
                }
            }

            ParamsTable {
                listModel: ListModel {
                    ListElement {
                        name: "id"
                        value: "1"
                        isChecked: true
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
                    text: "TextArea\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n"
                    wrapMode: TextArea.Wrap
                    selectByMouse: true
                    background: Rectangle {
                        border.color: "#EEEEEE"
                        border.width: 1
                        radius: 4
                    }
                }

                ScrollBar.vertical: ScrollBar { }
            }
        }

        ParamsTable {
            listModel: ListModel {
                ListElement {
                    name: "header1"
                    value: "value 1"
                    isChecked: true
                }
                ListElement {
                    name: "header2"
                    value: "value 2"
                    isChecked: true
                }
                ListElement {
                    name: "header3"
                    value: "value 3"
                    isChecked: false
                }
            }
        }
    }

}
