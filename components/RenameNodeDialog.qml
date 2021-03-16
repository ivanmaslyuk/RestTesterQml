import QtQuick 2.3
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12


Dialog {
    width: 400
    height: contentItem.implicitHeight
    title: qsTr("Переименовать")

    property var _parentIndex: null

    function show(parentIndex) {
        _parentIndex = parentIndex
        nameField.text = ""
        open()
        nameField.focus = true
    }

    contentItem: ColumnLayout {
        spacing: 12
        anchors.fill: parent

        Text {
            text: qsTr("Переименовать")
            font.pixelSize: 16
            Layout.topMargin: 12
            Layout.leftMargin: 16
        }

        TextField {
            id: nameField
            placeholderText: qsTr("Название")
            implicitHeight: 40
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.fillWidth: true
            selectByMouse: true

            background: Rectangle {
                anchors.fill: parent
                border.color: "#EEEEEE"
                radius: 4
            }
        }

        Rectangle { Layout.fillHeight: true }

        RowLayout {
            spacing: 12
            Layout.bottomMargin: 12
            Layout.leftMargin: 16
            Layout.rightMargin: 16

            Rectangle { Layout.fillWidth: true }

            CustomButton {
                text: "ОТМЕНА"
                emphasis: "medium"
                onClicked: close()
            }

            CustomButton {
                text: "СОХРАНИТЬ"
                onClicked: {
                    app.renameNode(nameField.text, _parentIndex)
                    close()
                }
            }
        }
    }
}
