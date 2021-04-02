import QtQuick 2.0
import QtQuick.Layouts 1.12


Rectangle {
    implicitHeight: 40
    implicitWidth: rootLayout.implicitWidth
                   + rootLayout.anchors.leftMargin + rootLayout.anchors.rightMargin
    color: "transparent"

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

        Text {
            font.pixelSize: 16
            text: "Proverb"
            Layout.alignment: Qt.AlignVCenter
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
