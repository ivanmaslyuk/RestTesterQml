import QtQuick 2.0
import QtQuick.Controls 2.15

Button {
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onPressed: mouse.accepted = false
    }
}
