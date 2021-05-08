import QtQuick 2.0
import QtGraphicalEffects 1.15



Rectangle {
    id: control
    width: 30
    height: width
    color: theme.secondaryBackground
    radius: width / 2

    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Rectangle {
            height: control.height
            width: control.width
            radius: control.width / 2
        }
    }

    Rectangle {
        id: triangle
        anchors.fill: parent
        color: "transparent"

        Canvas {
            anchors.fill:parent

            onPaint: {
                var context = getContext("2d");

                // the triangle
                context.beginPath();
                context.moveTo(0, 0);
                context.lineTo(0, parent.height);
                context.lineTo(parent.width / 2, parent.height / 2);
                context.closePath();

                // the fill color
                context.fillStyle = theme.accentColor;
                context.fill();
            }
        }
    }

    Rectangle {
        id: innerCircle
        color: theme.primaryBackground
        height: parent.height / 2
        width: parent.width / 2
        radius: parent.width / 2
        anchors.centerIn: parent
    }

    RotationAnimation {
        target: triangle
        from: 0
        to: 360
        duration: 500
        loops: Animation.Infinite
        running: true
    }

}
