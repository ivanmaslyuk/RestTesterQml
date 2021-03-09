import QtQuick 2.0
import QtQuick.Controls 2.15

Button {
    padding: 0
    font.underline: hovered
    font.pixelSize: 12
    background: Rectangle { color: "transparent" }

    property string color: "black"

    contentItem: Text {
        text: parent.text
        font: parent.font
        color: parent.color
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onPressed: mouse.accepted = false
    }
}
