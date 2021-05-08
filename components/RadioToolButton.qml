import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.15

Rectangle {
    id: control
    radius: 4
    height: 26
    color: on ? theme.toolButtonBg : "transparent"
    width: content.implicitWidth + 16

    property bool hovered: false
    property bool on: false
    property string text: ""

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onEntered: hovered = true
        onExited: hovered = false
        onPressed: on = !on
    }

    RowLayout {
        id: content
        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        spacing: 8

        Image {
            id: icon
            visible: false
            source: "/icons/sidebar.svg"
        }

        ColorOverlay {
            height: icon.height
            width: icon.width
            source: icon
            color: theme.toolButtonFg
        }

        Text {
            text: control.text
            font.pixelSize: 12
            color: theme.toolButtonFg
        }
    }
}
