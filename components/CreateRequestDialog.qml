import QtQuick 2.3
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12


Dialog {
    width: 400
    height: 400
    title: qsTr("Новый запрос")

    property var _parentIndex: null

    function show(parentIndex) {
        _parentIndex = parentIndex
        nameField.text = ""
        methodSelect.currentIndex = 0
        open()
        nameField.focus = true
    }

    contentItem: ColumnLayout {
        spacing: 12
        anchors.fill: parent

        Text {
            text: qsTr("Новый запрос")
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

        StyledComboBox {
            id: methodSelect
            implicitHeight: 40
            font.pixelSize: 16
            textRole: "text"
            valueRole: "value"
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.fillWidth: true

            model: [
                { text: "GET", value: "GET" },
                { text: "POST", value: "POST" },
                { text: "PUT", value: "PUT" },
                { text: "PATCH", value: "PATCH" },
                { text: "DELETE", value: "DELETE" },
            ]
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
                text: "СОЗДАТЬ"
                onClicked: {
                    app.createRequest(nameField.text, methodSelect.currentValue, _parentIndex)
                    close()
                }
            }
        }
    }
}
