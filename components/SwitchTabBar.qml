import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15

Rectangle {
    width: tabBar.width + 4
    height: tabBar.height + 4
    color: theme.switchTabBarBg
    radius: 4

    required property var tabsModel
    readonly property int currentIndex: tabBar.currentIndex

    TabBar {
        id: tabBar
        contentHeight: 24
        spacing: 2

        anchors.top: parent.top
        anchors.topMargin: 2
        anchors.left: parent.left
        anchors.leftMargin: 2

        background: Rectangle {
            color: "transparent"
        }

        Repeater {
            model: tabsModel

            TabButton {
                id: tabButton
                text: modelData
                height: 24
                width: contentItem.implicitWidth + 16

                background: Rectangle {
                    color: (tabButton == tabBar.currentItem) ? theme.primaryBackground : "transparent"
                    radius: 4
                }

                contentItem: Text {
                    font.pixelSize: 14
                    font.family: "Roboto"
                    text: parent.text
                    color: theme.textColor

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onPressed: mouse.accepted = false
                }
            }
        }
    }
}
