import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15

ColumnLayout {
    spacing: 0
    property bool isEditing: false

    RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 16 - 9  // 10px SplitView handle on the left
        Layout.topMargin: 12
        Layout.rightMargin: 16
        Layout.bottomMargin: 12

        Text {
            text: qsTr("Документация")
            color: theme.textColor
            Layout.alignment: Qt.AlignVCenter
        }

        Rectangle {
            Layout.fillWidth: true
        }

        LinkButton {
            text: isEditing ? qsTr("Готово") : qsTr("Изменить")
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
            color: theme.textColor

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
                id: documentationShow
                leftPadding: 16 - 9  // 10px SplitView handle on the left
                rightPadding: 16
                wrapMode: TextArea.Wrap
                selectByMouse: true
                placeholderText: qsTr("Нет документации")
                color: theme.textColor
                font.pixelSize: 12
                readOnly: true
                textFormat: TextArea.MarkdownText
                onLinkActivated: Qt.openUrlExternally(link)
                onLinkHovered: { }

                Connections {
                    target: app
                    function onActiveRequestChanged(request) {
                        documentationShow.text = request.documentation
                        documentationShow.update()
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {}
        }

        Flickable {
            boundsBehavior: Flickable.StopAtBounds

            TextArea.flickable: TextArea {
                id: documentationEdit
                leftPadding: 16 - 9  // 10px SplitView handle on the left
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

    }
}
