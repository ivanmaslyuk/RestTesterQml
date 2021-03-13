import QtQuick 2.0
import QtQuick.Controls 2.15

Button {
    id: control
    property string hoverColor: getHoverColor()
    property string color: getColor()
    property string textColor: getTextColor()
    property string emphasis: "high"
    property string _currentColor: color

    function getColor() {
        if (emphasis === "high")
            return "#3D5AFE"
        else if (emphasis === "medium")
            return "#EEEEEE"
    }

    function getHoverColor() {
        if (emphasis === "high")
            return "#304FFE"
        else if (emphasis === "medium")
            return "#cccccc"
    }

    function getTextColor() {
        if (emphasis === "high")
            return "white"
        else if (emphasis === "medium")
            return "#555555"
    }

    contentItem: Text {
        font.pixelSize: 16
        font.bold: true
        text: control.text
        color: getTextColor()

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: control._currentColor
        radius: 4
    }

    onHoveredChanged: {
        _currentColor = control.hovered ? control.hoverColor : control.color
    }

    Behavior on _currentColor {
        ColorAnimation { duration: 100; loops: 1 }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onPressed: mouse.accepted = false
    }

}
