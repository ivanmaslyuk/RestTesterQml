import QtQuick 2.0
import QtQuick.Layouts 1.12


Rectangle {
    implicitHeight: rootLayout.implicitHeight + rootLayout.anchors.topMargin
    implicitWidth: rootLayout.implicitWidth
                   + rootLayout.anchors.leftMargin + rootLayout.anchors.rightMargin

    ColumnLayout {
        id: rootLayout
        spacing: 12
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 12

        ColumnLayout {
            spacing: 4

            Text {
                font.pixelSize: 16
                text: "REST Tester"
            }

            RowLayout {
                spacing: 0

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
        }

        StyledComboBox {
            textRole: "text"
            valueRole: "value"
            font.pixelSize: 14
            implicitHeight: 26
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            model: [
                { value: "Reddit API", text: "Reddit API" },
                { value: "Reddit API", text: "VK API" },
                { value: "Reddit API", text: "YouTube API" },
                { value: "Reddit API", text: "Twitter API" },
            ]
        }

    }

}
