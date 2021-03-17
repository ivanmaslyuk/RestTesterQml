import QtQuick 2.0

Rectangle {
    id: control

    property string backgroundColor: "#EEEEEE"
    property string indicatorColor: "#2962FF"
    height: 3
    width: 100
    color: backgroundColor
    clip: true

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

    onWidthChanged: {
        anim1.running = false
        parallelAnim.running = false
        anim1.from = -rect.width
        anim1.to = control.width

        anim2.from = rect.width
        anim2.to = rect.width / 2

        anim3.from = rect.width / 2
        anim3.to = rect.width

        anim4.from = 0
        anim4.to = rect.width / 4

        anim5.from = rect.width / 4
        anim5.to = 0

        anim1.running = true
        parallelAnim.running = true
    }

    NumberAnimation {
        id: anim1
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
        id: parallelAnim
        running: true
        loops: Animation.Infinite

        SequentialAnimation {
            NumberAnimation {
                id: anim2
                target: nestedRect
                property: "width"
                from: rect.width
                to: rect.width / 2
                duration: _duration / 2
                easing.type: Easing.OutSine
            }
            NumberAnimation {
                id: anim3
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
                id: anim4
                target: nestedRect
                property: "x"
                from: 0
                to: rect.width / 4
                duration: _duration / 2
                easing.type: Easing.OutSine
            }
            NumberAnimation {
                id: anim5
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
