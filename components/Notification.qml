import QtQuick 2.15
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12

Rectangle {
    id: control
    anchors.fill: parent
    color: "transparent"
//    clip: true

    function showNotification(text) {
        listView.model.append({title: text})
        if (listView.model.count > 5)
            listView.model.remove(0)
    }



//    Repeater {
//        id: repeater
//        anchors.left: parent.left
//        anchors.top: parent.top
//        anchors.bottom: parent.bottom
//        width: 250

//        model: ListModel { }

//        property ListModel textHeights: ListModel {}

//        delegate: Rectangle {
//            id: notification
//            visible: false
//            clip: true
//            height: notificationText.contentHeight
//            width: 250
//            color: "#cc0000"
//            anchors.bottom: parent.bottom
//            anchors.left: parent.left
//            radius: 4
//            anchors.leftMargin: 16
//            anchors.bottomMargin: 12 + ((12 + height) * model.index)

//            Text {
//                id: notificationText
//                anchors.fill: parent
//                text: model.title
//                color: "white"
//                wrapMode: Text.Wrap
//                font.pixelSize: 16
//                verticalAlignment: Text.AlignVCenter
//                anchors.leftMargin: 16
//                anchors.rightMargin: 16
//            }

//            NumberAnimation {
//                target: notification
//                property: "anchors.bottomMargin"
//                from: -notification.height
//                to: 12
//                duration: 100
//                loops: 1
//                easing.type: Easing.OutBack
//                running: true
//            }
//        }
//    }




    ColumnLayout {
        id: listView
        property var model: ListModel { }
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.bottomMargin: 12
        spacing: 12

        Rectangle { Layout.fillHeight: true }

        Repeater {
            model: parent.model

            delegate: Rectangle {
                clip: true
                height: notificationText.contentHeight + 12 * 2
                width: 250
                color: "#cc0000"
                radius: 4

                Text {
                    id: notificationText
                    anchors.fill: parent
                    text: model.title
                    color: "white"
                    wrapMode: Text.Wrap
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: closeButton.visible = true
                    onExited: closeButton.visible = false

                    Rectangle {
                        id: closeButton
                        color: "#a70000"
                        visible: false
                        height: 24
                        width: 24
                        radius: 4
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.rightMargin: 6
                        anchors.topMargin: 6

                        Canvas {
                            id: xIcon
                            width: 10
                            height: 10
                            anchors.centerIn: parent
                            contextType: "2d"
                            onPaint: {
                                context.reset()
                                context.strokeStyle = "#ffffff"
                                context.moveTo(0, 0)
                                context.lineTo(width, height)
                                context.stroke()
                                context.moveTo(0, height)
                                context.lineTo(width, 0)
                                context.stroke()
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onEntered: parent.color = "#ff5252"
                            onExited: parent.color = "#a70000"
                            onClicked: listView.model.remove(parseInt(model.index))
                        }
                    }
                }
            }
        }
    }




}
