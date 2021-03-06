import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ComboBox {
    id: control
    font.pixelSize: 14
    property int sidePadding: 8
    property bool hideBackground: false

    delegate: ItemDelegate {
        width: Math.max(control.width, 120)
        height: control.height
        leftPadding: 8
        rightPadding: 8
        highlighted: control.highlightedIndex === index

        background: Rectangle {
            color: parent.highlighted ? theme.secondaryBackground : theme.primaryBackground
        }

        contentItem: Text {
            text: modelData.text
            color: theme.secondaryTextColor
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onPressed: mouse.accepted = false
        }
    }

    popup: Popup {
        y: control.height + 2
        x: (control.width / 2) - (width / 2)
        width: Math.max(parent.width, 120)
        implicitHeight: contentItem.implicitHeight + 2
        padding: 1

        contentItem: ListView {
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            delegate: Rectangle {
                color: "transparent"
                Text {
                    text: model.text
                }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: theme.lineColor
            radius: 4
            color: theme.primaryBackground
        }
    }

    contentItem: Text {
        text: control.currentText
        anchors.left: parent.left
        anchors.leftMargin: sidePadding

        verticalAlignment: Text.AlignVCenter
        font: control.font
        color: theme.secondaryTextColor
        anchors.verticalCenter: parent.verticalCenter

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onPressed: mouse.accepted = false
        }
    }

    indicator: Canvas {
        id: canvasId
        x: parent.contentItem.width - sidePadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 8
        height: 6
        contextType: "2d"

        Connections {
            target: control
            function onPressedChanged() { canvasId.requestPaint(); }
        }

        onPaint: {
            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = "#555555";
            context.fill();
        }
    }

    background: Rectangle {
        visible: !hideBackground
        implicitWidth: control.contentItem.implicitWidth + control.indicator.width
                       + 6 // indicator spacing
                       + sidePadding * 2
        height: parent.implicitHeight
        border.color: theme.lineColor
        border.width: 1
        radius: 4
        color: "transparent"
    }
}
