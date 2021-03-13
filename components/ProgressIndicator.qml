import QtQuick 2.0

Rectangle {
    id: control

    property string backgroundColor: "#EEEEEE"
    property string indicatorColor: "#2962FF"

    color: backgroundColor
    height: 3
    width: 800

    property int _duration: 1000

    Rectangle {
        id: rect
        color: "transparent"
        height: parent.height
        width: control.width / 3

        Rectangle {
            id: nestedRect
            height: parent.height
            width: parent.width
            color: indicatorColor
        }
    }

    NumberAnimation {
        target: rect
        property: "x"
        from: -rect.width
        to: control.width
        duration: _duration
        running: true
        loops: Animation.Infinite
        easing.type: Easing.OutInSine
    }

    ParallelAnimation {
        running: true
        loops: Animation.Infinite

        SequentialAnimation {
            NumberAnimation {
                target: nestedRect
                property: "width"
                from: rect.width
                to: rect.width / 2
                duration: _duration / 2
                easing.type: Easing.OutSine
            }
            NumberAnimation {
                target: nestedRect
                property: "width"
                from: rect.width / 2
                to: rect.width
                duration: _duration / 2
                easing.type: Easing.InSine
            }
        }

        SequentialAnimation {
            NumberAnimation {
                target: nestedRect
                property: "x"
                from: 0
                to: rect.width / 4
                duration: _duration / 2
                easing.type: Easing.OutSine
            }
            NumberAnimation {
                target: nestedRect
                property: "x"
                from: rect.width / 4
                to: 0
                duration: _duration / 2
                easing.type: Easing.InSine
            }
        }
    }


}
