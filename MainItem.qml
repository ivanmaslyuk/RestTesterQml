import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.15

import "components"

Rectangle {
    id: control
    anchors.fill: parent
    color: theme.primaryBackground

    Component.onCompleted: {
        theme.primaryBackground = "#121212"
        theme.secondaryBackground = "#181818"
        theme.accentColor = "#48B146"
        theme.textColor = "white"
        theme.lineColor = "#222222"
        theme.switchTabBarBg = "#282828"
        theme.treeViewHighlight = "#282828"
        theme.secondaryTextColor = "#808080"
        theme.toolButtonBg = "#333"
        theme.toolButtonFg = "white"
    }

    Connections {
        target: requestView
        function onNeedMoreSpace(addWidth) {
            control.width += addWidth
        }
    }

    Rectangle {
        height: 1
        color: theme.lineColor
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        z: 100
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        AppMenu {
            id: menu
            Layout.fillWidth: true
        }

        SplitView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            handle: Rectangle {
                id: splitViewHandle
                implicitWidth: 10
                color: theme.secondaryBackground

                Rectangle {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    color: theme.lineColor
                    width: 1
                }
            }

            CustomTreeView {
                id: treeView
                clip: true
                visible: app.showRequestList
                SplitView.minimumWidth: 100
                SplitView.maximumWidth: control.width / 3
            }

            RequestView {
                id: requestView
            }
        }
    }

    Notification {
        id: notificationPresenter

        Connections {
            target: app.serverSyncService
            function onSyncError(err) {
                notificationPresenter.showNotification(err)
            }
            function onSyncFinished() {
                notificationPresenter.showNotification(qsTr('Синхронизация завершена'))
            }
        }
    }
}
