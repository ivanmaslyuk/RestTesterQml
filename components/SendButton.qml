import QtQuick 2.0
import QtGraphicalEffects 1.15

Rectangle {
    id: control
    width: 52
    height: 40
    radius: 4
    border.color: theme.accentColor
    color: "transparent"

    property bool hovered: false

    function onClicked() { }

    states: State {
        name: "hovered"
        when: control.hovered
        PropertyChanges { target: background; opacity: 0.4 }
    }

    transitions: Transition {
        NumberAnimation { target: background; property: "opacity"; duration: 100 }
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: theme.accentColor
        radius: 4
        opacity: 0.3
    }

    Image {
        id: sendIcon
        visible: false
        width: 24
        height: 24
        source: "/icons/send.svg"
        anchors.centerIn: parent
    }

    ColorOverlay {
        anchors.fill: sendIcon
        source: sendIcon
        color: theme.accentColor
        transform:rotation
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true

        onEntered: control.hovered = true
        onExited: control.hovered = false
        onClicked: control.onClicked()
    }
}
