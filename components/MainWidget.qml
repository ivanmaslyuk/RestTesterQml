import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "."

ColumnLayout  {
    id: rootLayout
    spacing: 12

    property var name: "Create User"
    property var url: "http://example.com/?q=red+cars"

    Text {
        text: name
        font.family: "Roboto"
        font.pixelSize: 16
    }

    RowLayout {
        spacing: 12
        Layout.preferredHeight: 40
        Layout.maximumHeight: 40
        Layout.minimumHeight: 40

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

                    background: Rectangle {
//                        color: "red"
                    }

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

    Rectangle {
        width: tabBar.width + 4
        height: tabBar.height + 4
        color: "#EEEEEE"
        radius: 4

        TabBar {
            id: tabBar
            contentHeight: 24
            spacing: 2

            anchors.top: parent.top
            anchors.topMargin: 2
            anchors.left: parent.left
            anchors.leftMargin: 2

            background: Rectangle {
                color: "#EEEEEE"
            }

            Repeater {
                model: ["Query", "Body", "Headers"]

                TabButton {
                    id: tabButton
                    text: modelData
                    height: 24
                    width: contentItem.implicitWidth + 16

                    background: Rectangle {
                        color: (tabButton == tabBar.currentItem) ? "white" : "#EEEEEE"
                        radius: 4
                    }

                    contentItem: Text {
                        font.pixelSize: 14
                        font.family: "Roboto"
                        text: parent.text

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }

    StackLayout {
        currentIndex: tabBar.currentIndex

        ParamsTable { }

        Text {
            text: "2222222222222"
        }

        Text {
            text: "33333333333333333"
        }
    }

}
