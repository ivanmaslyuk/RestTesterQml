import QtQuick 2.0

Rectangle {
    width: 400
    height: 400
    color: "transparent"

    Rectangle {
        id: notification
        height: 50
        width: 250
        color: "#555555"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        radius: 4
        anchors.leftMargin: 16
        anchors.bottomMargin: 12

        Text {
            anchors.fill: parent
            text: "Notification text"
            color: "white"
            font.pixelSize: 16
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 16
            anchors.rightMargin: 16
        }
    }

    NumberAnimation {
        target: notification
        property: "anchors.bottomMargin"
        from: -notification.height
        to: 12
        duration: 100
        loops: 1
        easing.type: Easing.OutBack
        running: true
    }
}
