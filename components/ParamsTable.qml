import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15
import QtQml.Models 2.15


Rectangle {
    id: rootRect
    color: "transparent"
    property bool showCheckBox: true
    property bool readOnly: false
    property bool appendEmptyRow: true
    property var model: null

    property int _hoveredRowIndex: -1

    signal rowDeleted()

    function modelRowCount() {
        if (!model) return 0
        return model.rowCount()
    }

    function fullTableVisible() {
        return rootRect.height >= control.implicitHeight
    }

    Connections {
        target: model
        function onEmptyRowAppended() {
            if (!fullTableVisible())
                flickable.scrollDown()
        }
    }

    Rectangle {
        anchors.fill: parent
        border.color: theme.lineColor
        border.width: 1
        radius: 4
        color: "transparent"
        z: 1
        visible: !fullTableVisible()
    }

    Flickable {
        id: flickable
        clip: true
        contentHeight: control.implicitHeight
        boundsBehavior: Flickable.StopAtBounds
        anchors.fill: parent

        ScrollBar.vertical: ScrollBar { }

        function scrollDown() {
            ScrollBar.vertical.position = 1
        }

        Rectangle {
            id: control
            color: "transparent"
            border.color: fullTableVisible() ? theme.lineColor : "transparent"
            border.width: 1
            radius: 4
            anchors.fill: parent
            implicitHeight: rootLayout.implicitHeight + rootLayout.anchors.margins * 2
            clip: true

            RowLayout {
                id: rootLayout
                spacing: 0

                anchors.fill: parent
                anchors.margins: 1

                ColumnLayout {
                    id: heightHack  // bad code
                    spacing: 1
                    Repeater {
                        model: rootRect.model
                        Rectangle { implicitHeight: 30 }
                    }
                }

                ColumnLayout {
                    id: checkBoxCoulmn
                    spacing: 0
                    visible: showCheckBox

                    Repeater {
                        model: rootRect.model

                        ColumnLayout {
                            spacing: 0

                            Rectangle {
                                implicitHeight: index == 0 ? 0 : 1
                                implicitWidth: 36
                                color: theme.lineColor
                            }

                            CheckDelegate {
                                id: paramCheckBox
                                implicitWidth: 30
                                implicitHeight: 30
                                padding: 0
                                Layout.alignment: Qt.AlignCenter
                                enabled: !readOnly

                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onEntered: _hoveredRowIndex = model.index
                                    onExited: _hoveredRowIndex = -1
                                    acceptedButtons: Qt.NoButton
                                    cursorShape: Qt.PointingHandCursor
                                }

                                indicator: Rectangle {
                                    implicitHeight: 12
                                    implicitWidth: 12
                                    color: "transparent"
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

                                background: Rectangle {
                                    color: "transparent"

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor

                                        onReleased: {
                                            paramCheckBox.checked = !paramCheckBox.checked
                                        }
                                    }
                                }

                                onCheckedChanged: {
                                    model.isEnabled = checked
                                }

                                Component.onCompleted: {
                                    paramCheckBox.checked = model.isEnabled
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    visible: showCheckBox
                    implicitWidth: 1
                    Layout.fillHeight: true
                    color: theme.lineColor
                }

                SplitView {
                    id: splitView
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    handle: Rectangle {
                        id: separator
                        implicitWidth: 13
                        color: "transparent"

                        property int rowHeight: 30

                        ColumnLayout {
                            spacing: 0
                            anchors.fill: parent

                            Repeater {
                                model: rootRect.model

                                ColumnLayout {
                                    spacing: 0

                                    Rectangle {
                                        implicitWidth: separator.implicitWidth
                                        implicitHeight: index == 0 ? 0 : 1
                                        color: theme.lineColor
                                    }

                                    Rectangle {
                                        implicitWidth: 1
                                        implicitHeight: rowHeight
                                        color: theme.lineColor
                                        Layout.alignment: Qt.AlignHCenter
                                    }
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: nameColumn
                        SplitView.minimumWidth: 50
                        SplitView.preferredWidth: splitView.width / 4
                        color: "transparent"

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 0

                            Repeater {
                                model: rootRect.model

                                ColumnLayout {
                                    spacing: 0

                                    Rectangle {
                                        height: index == 0 ? 0 : 1
                                        Layout.fillWidth: true
                                        color: theme.lineColor
                                    }

                                    TextField {
                                        id: keyField
                                        implicitHeight: 30
                                        leftPadding: 12
                                        rightPadding: 6
                                        selectByMouse: true
                                        Layout.fillWidth: true
                                        font.family: "Roboto"
                                        font.pixelSize: 12
                                        readOnly: rootRect.readOnly
                                        placeholderText: "Key"
                                        color: theme.textColor

                                        background: Rectangle {
                                            color: "transparent"
                                        }

                                        onTextChanged: {
                                            model.key = text
                                        }

                                        Component.onCompleted: {
                                            keyField.text = model.key
                                        }

                                        MouseArea {
                                            anchors.fill: parent
                                            hoverEnabled: true
                                            onEntered: _hoveredRowIndex = model.index
                                            onExited: _hoveredRowIndex = -1
                                            acceptedButtons: Qt.NoButton
                                            cursorShape: Qt.IBeamCursor
                                        }
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
                                model: rootRect.model

                                ColumnLayout {
                                    spacing: 0

                                    Rectangle {
                                        implicitHeight: index == 0 ? 0 : 1
                                        Layout.fillWidth: true
                                        color: theme.lineColor
                                    }

                                    TextField {
                                        id: valueField
                                        implicitHeight: 30
                                        leftPadding: 6
                                        rightPadding: 12
                                        selectByMouse: true
                                        Layout.fillWidth: true
                                        font.family: "Roboto"
                                        font.pixelSize: 12
                                        readOnly: rootRect.readOnly
                                        placeholderText: "Value"
                                        color: theme.textColor
                                        onTextChanged: model.value = text
                                        Component.onCompleted: valueField.text = model.value

                                        background: Rectangle {
                                            color: "transparent"
                                        }

                                        MouseArea {
                                            anchors.fill: parent
                                            hoverEnabled: true
                                            onEntered: _hoveredRowIndex = model.index
                                            onExited: _hoveredRowIndex = -1
                                            acceptedButtons: Qt.NoButton
                                            cursorShape: Qt.IBeamCursor
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: deleteButtonColumn
                visible: !readOnly
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.topMargin: 1
                anchors.bottomMargin: 1
                anchors.rightMargin: 12
                width: 30
                color: "transparent"

                Component {
                    id: deleteButton

                    Rectangle {
                        id: deleteButtonContent
                        width: 30
                        height: 30
                        color: "transparent"
                        visible: index !== modelRowCount() - 1

                        Connections {
                            target: rootRect
                            function on_hoveredRowIndexChanged() {
                                deleteButtonContent.visible = index !== modelRowCount() - 1
                            }
                        }

                        property bool hovered: false

                        Rectangle {
                            id: deleteButtonBackground
                            width: 20
                            height: 20
                            anchors.centerIn: parent
                            color: parent.hovered ? theme.treeViewHighlight : "transparent"
                            radius: 4
                        }

                        MouseArea {
                            anchors.fill: deleteButtonBackground
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            onEntered: parent.hovered = true
                            onExited: parent.hovered = false
                            preventStealing: true
                            onClicked: {
                                if (model.isEnabled)
                                    model.isEnabled = false

                                rootRect.rowDeleted()
                                rootRect.model.removeRows(index, 1)
                            }
                        }

                        Canvas {
                            id: xIcon
                            width: 10
                            height: 10
                            anchors.centerIn: parent
                            contextType: "2d"
                            visible: _hoveredRowIndex === index || parent.hovered
                            onPaint: {
                                context.reset()
                                context.strokeStyle = "#ffffff"
                                context.moveTo(0, 0)
                                context.lineTo(width, height)
                                context.stroke()
                                context.moveTo(0, height)
                                context.lineTo(width, 0)
                                context.stroke()
                            }
                        }
                    }
                }

                ColumnLayout {
                    spacing: 1

                    Repeater {
                        model: rootRect.model
                        delegate: deleteButton
                    }
                }
            }
        }
    }
}
