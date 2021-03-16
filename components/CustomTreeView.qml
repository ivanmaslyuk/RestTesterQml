import QtQuick 2.0
import QtQuick.Controls 1.6
import QtQuick.Layouts 1.12
import QtQml.Models 2.15

TreeView {
    id: control
    headerVisible: false
    model: app.requestTreeModel
    frameVisible: false

    property int itemHeight: 24

    TableViewColumn {
        title: "Name"
        role: "name"
    }

    CreateRequestDialog { id: createDialog }
    CreateFolderDialog { id: createFolderDialog }
    RenameNodeDialog { id: renameNodeDialog }

    Menu {
        id: contextMenu
        property var _index: null
        property bool _showNodeActions: true

        function show(index, showNodeActions = true) {
            contextMenu._index = index
            contextMenu._showNodeActions = showNodeActions
            popup()
        }

        MenuItem {
            visible: contextMenu._showNodeActions
            text: qsTr("Переименовать")
            onTriggered: renameNodeDialog.show(contextMenu._index)
        }
        MenuItem {
            text: qsTr("Создать папку")
            onTriggered: createFolderDialog.show(contextMenu._index)
        }
        MenuItem {
            text: qsTr("Создать запрос")
            onTriggered: createDialog.show(contextMenu._index)
        }
        MenuItem {
            visible: contextMenu._showNodeActions
            text: qsTr("Удалить")
            onTriggered: app.deleteNode(contextMenu._index)
        }
    }

    itemDelegate: Rectangle {
        color: "transparent"

        RowLayout {
            anchors.fill: parent
            spacing: 4

            Text {
                font.pixelSize: 14
                font.bold: true
                color: "orange"
                text: model.method ? model.method : ""
            }

            Text {
                id: nameText
                font.pixelSize: 14
                text: styleData.value
                color: (control.activeFocus && styleData.selected) ? "white" : "black"
                Layout.fillWidth: true
            }
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.RightButton
            onClicked: contextMenu.show(styleData.index)
            z: 100
        }
    }

    rowDelegate: Rectangle {
        function getColor() {
            if (!styleData.selected) return "transparent"

            if (control.activeFocus) {
                return "#424242"
            }

            return "#EEEEEE"
        }

        id: rowDelegate
        color: getColor()
        height: itemHeight
    }

    onActivated: {
        app.requestTreeItemActivated(index)
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: contextMenu.show(rootIndex, false)
        z: -1
    }
}
