import QtQuick 2.0
import QtQuick.Controls 2.15

Button {
    id: control
    property string emphasis: "high"
    property string color: theme.accentColor
    property bool secondary: false

    states: [

        State {
            name: "disabled"
            when: !control.enabled
            PropertyChanges {
                target: buttonBorderBox
                border.width: 0
            }
            PropertyChanges {
                target: buttonBackground
                opacity: 0.3
            }
        },
        State {
            name: "regular-secondary"
            when: !hovered && secondary
            PropertyChanges {
                target: buttonBackground
                opacity: 0
            }
        },
        State {
            name: "regular-primary"
            when: !hovered && !secondary
            PropertyChanges {
                target: buttonBackground
                opacity: 0.3
            }
        },
        State {
            name: "hovered"
            when: hovered && !pressed
            PropertyChanges {
                target: buttonBackground
                opacity: 0.4
            }
        },
        State {
            name: "pressed"
            when: pressed
            PropertyChanges {
                target: buttonBackground
                opacity: 0.5
            }
        }
    ]

    transitions: Transition {
        NumberAnimation {
            duration: 100
        }
    }

    contentItem: Text {
        id: buttonText
        font: parent.font
        text: control.text
        color: "white"

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        id: buttonBorderBox
        radius: 4
        border.width: control.secondary ? 0 : 1
        border.color: control.color
        color: "transparent"

        Rectangle {
            id: buttonBackground
            anchors.fill: parent
            radius: 4
            color: control.color
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onPressed: mouse.accepted = false
    }

}
