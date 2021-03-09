import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15

ColumnLayout {
    spacing: 0
    property bool isEditing: false

    RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.topMargin: 12
        Layout.rightMargin: 16
        Layout.bottomMargin: 12

        Text {
            text: qsTr("Документация")
            Layout.alignment: Qt.AlignVCenter
        }

        Rectangle {
            Layout.fillWidth: true
        }

        LinkButton {
            text: isEditing ? qsTr("Готово") : qsTr("Изменить")
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

            onClicked: {
                if (isEditing) {
                    app.activeRequest.documentation = documentationEdit.text
                }

                isEditing = !isEditing
            }
        }
    }

    StackLayout {
        currentIndex: isEditing ? 1 : 0

        Flickable {
            boundsBehavior: Flickable.StopAtBounds

            TextArea.flickable: TextArea {
                leftPadding: 16
                rightPadding: 16
                wrapMode: TextArea.Wrap
                selectByMouse: true
                placeholderText: qsTr("Нет документации")
                font.pixelSize: 12
                readOnly: true
                text: app.activeRequest.documentation
                textFormat: TextArea.MarkdownText
                onLinkActivated: Qt.openUrlExternally(link)
                onLinkHovered: { }
            }

            ScrollBar.vertical: ScrollBar {}
        }

        Flickable {
            boundsBehavior: Flickable.StopAtBounds

            TextArea.flickable: TextArea {
                id: documentationEdit
                leftPadding: 16
                rightPadding: 16
                wrapMode: TextArea.Wrap
                selectByMouse: true
                placeholderText: qsTr("Опишите запрос здесь")
                font.pixelSize: 12
                font.family: "Consolas"

                Connections {
                    target: app
                    function onActiveRequestChanged(request) {
                        documentationEdit.text = request.documentation
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {}
        }

//        ScrollView {
//            leftPadding: 16
//            rightPadding: leftPadding
//            bottomPadding: 12
//            clip: true

//            TextEdit {
//                text: app.activeRequest.documentation
//                textFormat: TextEdit.MarkdownText
//                font.pixelSize: 12
//                selectByMouse: true
//                readOnly: true
//                onLinkActivated: Qt.openUrlExternally(link)
//            }
//        }

//        ScrollView {
//            leftPadding: 16
//            rightPadding: leftPadding
//            bottomPadding: 12
//            clip: true

//            TextEdit {
//                id: documentationTextEdit
//                text: app.activeRequest.documentation
//                textFormat: TextEdit.PlainText
//                selectByMouse: true

//            }
//        }
    }
}
