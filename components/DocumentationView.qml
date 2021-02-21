import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15

ColumnLayout {
    spacing: 0
    property bool isEditing: false
    property string docString: "# Title goes here\n[Hello World](http://example.com)"

    RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.topMargin: 12
        Layout.rightMargin: 16
        Layout.bottomMargin: 12

        Text {
            text: qsTr("Documentation")
            Layout.alignment: Qt.AlignVCenter
        }

        Rectangle {
            Layout.fillWidth: true
        }

        Button {
            padding: 0
            font.underline: hovered
            font.pixelSize: 12
            text: isEditing ? qsTr("Done") : qsTr("Edit")
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
            background: Rectangle { }

            onClicked: {
                isEditing = !isEditing
            }
        }
    }

    StackLayout {
        currentIndex: isEditing ? 1 : 0

        ScrollView {
            leftPadding: 16
            rightPadding: leftPadding
            bottomPadding: 12
            clip: true

            TextEdit {
                text: docString
                textFormat: TextEdit.MarkdownText
                font.pixelSize: 12
                selectByMouse: true
                readOnly: true
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }

        ScrollView {
            leftPadding: 16
            rightPadding: leftPadding
            bottomPadding: 12
            clip: true

            TextEdit {
                text: docString
                textFormat: TextEdit.PlainText
                selectByMouse: true
            }
        }
    }
}
