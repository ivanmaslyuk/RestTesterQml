import QtQuick 2.0
import QtQuick.Controls 1.6
import QtQuick.Layouts 1.12

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
            onClicked: contextMenu.popup()

            Menu {
                id: contextMenu
                MenuItem {
                    text: qsTr("Переименовать")
                }
                MenuItem {
                    text: qsTr("Создать папку")
                    onTriggered: createFolderDialog.show(styleData.index)
                }
                MenuItem {
                    text: qsTr("Создать запрос")
                    onTriggered: createDialog.show(styleData.index)
                }
                MenuItem {
                    text: qsTr("Удалить")
                    onTriggered: console.log(styleData.index, 'delete')
                }
            }
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
}
