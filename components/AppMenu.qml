import QtQuick 2.0
import QtQuick.Layouts 1.12


Rectangle {
    implicitHeight: 40
    implicitWidth: rootLayout.implicitWidth
                   + rootLayout.anchors.leftMargin + rootLayout.anchors.rightMargin
    color: "transparent"

    RowLayout {
        id: rootLayout
        spacing: 12
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16

        Text {
            font.pixelSize: 16
            text: "REST Tester"
            Layout.alignment: Qt.AlignVCenter
        }

        RowLayout {
            spacing: 0
            Layout.alignment: Qt.AlignVCenter

            Text {
                font.pixelSize: 10
                text: "Ivan Maslyuk"
            }

            Text {
                font.pixelSize: 10
                text: " - "
            }

            LinkButton {
                font.pixelSize: 10
                color: "#777777"
                text: qsTr("Выйти")
            }
        }

        Rectangle {
            Layout.fillWidth: true
        }

        StyledComboBox {
            textRole: "text"
            valueRole: "value"
            Layout.alignment: Qt.AlignVCenter
            font.pixelSize: 14
            implicitHeight: 26
            implicitWidth: 200
            model: [
                { value: "Reddit API", text: "Reddit API" },
                { value: "Reddit API", text: "VK API" },
                { value: "Reddit API", text: "YouTube API" },
                { value: "Reddit API", text: "Twitter API" },
            ]
        }

    }

    Rectangle {
        implicitHeight: 1
        color: "#EEEEEE"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

}
