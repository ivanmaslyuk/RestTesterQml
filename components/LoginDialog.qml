import QtQuick 2.3
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12

Dialog {
    id: control
    width: 400
    height: 260
    title: qsTr("Авторизация")

    property bool invalidCredentials: false

    onVisibleChanged: {
        invalidCredentials = false
    }

    contentItem: ColumnLayout {
        spacing: 12
        anchors.fill: parent

        Text {
            text: qsTr("Авторизация")
            font.pixelSize: 16
            Layout.topMargin: 12
            Layout.leftMargin: 16
        }

        TextField {
            id: urlField
            placeholderText: qsTr("Адрес сервера")
            text: 'http://localhost:8000'
            implicitHeight: 40
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.fillWidth: true
            selectByMouse: true

            background: Rectangle {
                anchors.fill: parent
                border.color: control.invalidCredentials ? "red" : "#EEEEEE"
                radius: 4
            }
        }

        TextField {
            id: usernameField
            placeholderText: qsTr("Имя пользователя")
            implicitHeight: 40
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.fillWidth: true
            selectByMouse: true

            background: Rectangle {
                anchors.fill: parent
                border.color: control.invalidCredentials ? "red" : "#EEEEEE"
                radius: 4
            }
        }

        TextField {
            id: passwordField
            placeholderText: qsTr("Пароль")
            implicitHeight: 40
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.fillWidth: true
            selectByMouse: true
            echoMode: TextInput.Password

            background: Rectangle {
                anchors.fill: parent
                border.color: control.invalidCredentials ? "red" : "#EEEEEE"
                radius: 4
            }
        }

        Rectangle { Layout.fillHeight: true }

        RowLayout {
            spacing: 12
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 16
            Layout.bottomMargin: 12
            Layout.leftMargin: 16

            Spinner {
                id: spinner
                visible: false
                width: 20
            }

            Rectangle { Layout.fillWidth: true }

            CustomButton {
                text: "Войти"
                onClicked: {
                    spinner.visible = true
                    app.authenticator.logIn(urlField.text, usernameField.text,
                                            passwordField.text)
                }
            }
        }
    }

    Connections {
        target: app.authenticator
        function onLoginStatusChanged(loggedIn) {
            spinner.visible = false
            control.invalidCredentials = !loggedIn

            if (loggedIn)
                control.close()
        }
    }

}
