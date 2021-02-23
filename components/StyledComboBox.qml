import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ComboBox {
    id: control
    font.pixelSize: 14
    property int indicatorSpacing: 6
    property int sidePadding: 8

    delegate: ItemDelegate {
        width: 120
        leftPadding: sidePadding
        rightPadding: sidePadding
        highlighted: control.highlightedIndex === index

        contentItem: Text {
            text: modelData
            color: "#555555"
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    popup: Popup {
        y: control.height + 2
        x: control.implicitWidth - width
        width: 120
        implicitHeight: contentItem.implicitHeight + 2
        padding: 1

        contentItem: ListView {
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: "#EEEEEE"
            radius: 4
        }
    }

    contentItem: Text {
        text: control.currentText
        anchors.left: parent.left
        anchors.leftMargin: sidePadding

        verticalAlignment: Text.AlignVCenter
        font: control.font
        color: "#555555"
        anchors.verticalCenter: parent.verticalCenter
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
        implicitWidth: control.contentItem.implicitWidth + control.indicator.width
                       + indicatorSpacing + sidePadding * 2
        height: parent.implicitHeight
        border.color: "#EEEEEE"
        border.width: 1
        radius: 4
    }
}
