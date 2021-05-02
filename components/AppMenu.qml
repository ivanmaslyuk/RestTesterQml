import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.1


Rectangle {
    id: control
    implicitHeight: 40
    implicitWidth: rootLayout.implicitWidth
                   + rootLayout.anchors.leftMargin + rootLayout.anchors.rightMargin
    color: "transparent"

    property bool showTree: true

    LoginDialog {
        id: loginDialog
        visible: false
    }

    RowLayout {
        id: rootLayout
        spacing: 12
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16

        RadioToolButton {
            id: showTreeButton
            text: qsTr("Список")
            on: showTree

            Binding {
                target: control
                property: "showTree"
                value: showTreeButton.on
            }
        }

        MessageDialog {
            id: syncUnsavedRequestsDialog
            title: qsTr("Синхронизация")
            icon: StandardIcon.Question
            text: qsTr("При синхронизации будут утеряны изменения в редактируемых запросах. Продолжить?")
            standardButtons: StandardButton.Yes | StandardButton.No
            onYes: {
                syncButton.enabled = false
                app.startSync()
            }
        }

        CustomButton {
            id: syncButton
            text: "Синхронизация"
            onClicked: {
                if (app.hasUnsavedRequests()) {
                    syncUnsavedRequestsDialog.open()
                } else {
                    syncButton.enabled = false
                    app.startSync()
                }
            }

            Connections {
                target: app.serverSyncService
                function onSyncError(errorMessage) {
                    syncButton.enabled = true
                }
                function onSyncFinished() {
                    syncButton.enabled = true
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
        }

        RowLayout {
            visible: app.authenticator.loggedIn
            spacing: 0
            Layout.alignment: Qt.AlignVCenter

            Text {
                id: usernameText
                font.pixelSize: 12
                text: app.settings.get('username')

                Connections {
                    target: app.authenticator
                    function onLoginStatusChanged(loggedIn) {
                        usernameText.text = app.settings.get('username')
                    }
                }
            }

            Text {
                font.pixelSize: 12
                text: " - "
            }

            LinkButton {
                font.pixelSize: 12
                color: "#777777"
                text: qsTr("Выйти")
                onClicked: app.authenticator.logOut()
            }
        }

        LinkButton {
            visible: !app.authenticator.loggedIn
            font.pixelSize: 12
            color: "#777777"
            text: qsTr("Войти")
            onClicked: loginDialog.open()
        }
    }

    Rectangle {
        implicitHeight: 1
        color: "#EEEEEE"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

}
