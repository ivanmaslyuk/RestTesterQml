import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15


ScrollView {
    clip: true
    property ListModel listModel: ListModel {
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

    Rectangle {
        id: control
        border.color: "#EEEEEE"
        border.width: 1
        radius: 4
        anchors.fill: parent
        implicitHeight: rootLayout.implicitHeight + 2

        RowLayout {
            id: rootLayout
            spacing: 0

            anchors.fill: parent
            anchors.margins: 1

            ColumnLayout {
                id: checkBoxCoulmn
                spacing: 0

                Repeater {
                    model: listModel

                    ColumnLayout {
                        spacing: 0

                        CheckDelegate {
                            id: paramCheckBox
                            checked: isChecked
                            implicitWidth: 30
                            implicitHeight: 30
                            padding: 0
                            Layout.alignment: Qt.AlignCenter

                            indicator: Rectangle {
                                implicitHeight: 12
                                implicitWidth: 12
                                color: "#F7F7F7"
                                anchors.centerIn: parent
                                radius: 2
                                border.width: 1
                                border.color: "#AAAAAA"

                                Rectangle {
                                    implicitHeight: 6
                                    implicitWidth: 6
                                    color: paramCheckBox.checked ? "#AAAAAA" : "transparent"
                                    radius: 2
                                    anchors.centerIn: parent
                                }
                            }

                            background: Rectangle { }
                        }

                        Rectangle {
                            implicitHeight: (index + 1 == listModel.count) ? 0 : 1
                            implicitWidth: 36
                            color: "#EEEEEE"
                        }
                    }
                }
            }

            Rectangle {
                implicitWidth: 1
                Layout.fillHeight: true
                color: "#EEEEEE"
            }

            SplitView {
                id: splitView
                Layout.fillWidth: true
                Layout.fillHeight: true

                handle: Rectangle {
                    id: separator
                    implicitWidth: 13

                    property int rowHeight: 30

                    ColumnLayout {
                        spacing: 0
                        anchors.fill: parent

                        Repeater {
                            model: listModel

                            ColumnLayout {
                                spacing: 0

                                Rectangle {
                                    implicitWidth: 1
                                    implicitHeight: rowHeight
                                    color: "#EEEEEE"
                                    Layout.alignment: Qt.AlignHCenter
                                }

                                Rectangle {
                                    implicitWidth: separator.implicitWidth
                                    implicitHeight: 1
                                    color: "#EEEEEE"
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    id: nameColumn
                    SplitView.minimumWidth: 50
                    SplitView.preferredWidth: splitView.width / 3
                    color: "transparent"

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        Repeater {
                            model: listModel

                            ColumnLayout {
                                spacing: 0

                                TextField {
                                    implicitHeight: 30
                                    leftPadding: 12
                                    rightPadding: 6
                                    selectByMouse: true
                                    Layout.fillWidth: true
                                    font.family: "Roboto"
                                    font.pixelSize: 12
                                    text: name

                                    background: Rectangle {
                                        color: "transparent"
                                    }
                                }

                                Rectangle {
                                    implicitHeight: (index + 1 == listModel.count) ? 0 : 1
                                    Layout.fillWidth: true
                                    color: "#EEEEEE"
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    id: valueColumn
                    SplitView.minimumWidth: 50
                    SplitView.fillWidth: true
                    color: "transparent"

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        Repeater {
                            model: listModel

                            ColumnLayout {
                                spacing: 0
                                TextField {
                                    implicitHeight: 30
                                    leftPadding: 6
                                    rightPadding: 12
                                    selectByMouse: true
                                    Layout.fillWidth: true
                                    font.family: "Roboto"
                                    font.pixelSize: 12
                                    text: value

                                    background: Rectangle {
                                        color: "transparent"
                                    }
                                }

                                Rectangle {
                                    implicitHeight: (index + 1 == listModel.count) ? 0 : 1
                                    Layout.fillWidth: true
                                    color: "#EEEEEE"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
