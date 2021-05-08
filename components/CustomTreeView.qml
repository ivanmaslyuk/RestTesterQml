import QtQuick 2.0
import QtQuick.Controls 1.6
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import QtQml.Models 2.15
import QtGraphicalEffects 1.15

TreeView {
    id: control
    headerVisible: false
    model: app.requestTreeModel
    frameVisible: false

    Image {
        id: branchIcon
        visible: false
        source: "/icons/tree_branch.svg"
    }

    style: TreeViewStyle {
        transientScrollBars: true
        backgroundColor: theme.secondaryBackground
        indentation: 0

        branchDelegate: ColorOverlay {
            width: 8
            height: 8
            x: 16 + 12 * styleData.depth + width
            rotation: styleData.isExpanded ? 90 : 0
            source: branchIcon
            color: theme.textColor
        }
    }

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
            anchors.leftMargin: 16 + 12 * styleData.depth
                                + (model.isFolder ? 8 : 0) // Last number is branchDelegate.width
            spacing: 4

            Text {
                font.pixelSize: 14
                font.bold: true
                color: getMethodColor(model.method)
                text: model.isFolder ? "" : model.method

                function getMethodColor(method) {
                    switch (method) {
                    case "GET":
                        return "#1565C0"
                    case "POST":
                        return "#EF6C00"
                    case "DELETE":
                        return "#D84315"
                    case "PUT":
                        return "#2E7D32"
                    case "PATCH":
                        return "#558B2F"
                    default:
                        return "#4527A0"
                    }
                }
            }

            Text {
                id: nameText
                font.pixelSize: 14
                color: theme.textColor
                text: styleData.value
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
        id: rowDelegate
        color: (styleData.selected | _hovered) ? theme.treeViewHighlight : "transparent"
        height: itemHeight

        property bool _hovered: false

        onVisibleChanged: _hovered = false

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            hoverEnabled: true

            onEntered: rowDelegate._hovered = true
            onExited: rowDelegate._hovered = false
        }
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
