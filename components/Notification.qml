import QtQuick 2.15
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

Rectangle {
    id: control
    anchors.fill: parent
    color: "transparent"

    function showNotification(text) {
        listView.model.append({title: text})
        if (listView.model.count > 5)
            listView.model.remove(0)
    }

//    Button {
//        text: "add notification"
//        onClicked: showNotification("fjskjfl fskjdf ldskfs lfkd jslkf skdjf skfdsjf")
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
                height: notificationText.contentHeight + 12 * 2
                width: 250
                color: "transparent"

                Rectangle {
                    id: notification
                    clip: true
                    color: "#cc0000"
                    radius: 4
                    height: parent.height
                    width: parent.width

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

                    NumberAnimation on y {
                        from: 100
                        to: 0
                        duration: 200
                        easing.type: Easing.InOutBack

                    }
                }

                DropShadow {
                    anchors.fill: notification
                    source: notification
                    color: "#22000000"
                    verticalOffset: 4
                    radius: 10
                }
            }
        }




    }




}
