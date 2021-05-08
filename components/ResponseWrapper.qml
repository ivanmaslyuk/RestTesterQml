import QtQuick 2.0
import QtQuick.Layouts 1.12

StackLayout {
    clip: true

    Connections {
        target: app.httpClient
        function onRequestStarted() {
            currentIndex = 1
        }
        function onResponseUpdated() {
            currentIndex = 2
        }
    }

    Connections {
        target: app
        function onActiveRequestChanged(request) {
            currentIndex = 0
        }
    }

    Rectangle {
        id: noResponseView
        color: theme.primaryBackground

        Text {
            text: "Нажмите «Отправить», чтобы получить ответ"
            anchors.centerIn: parent
            color: theme.textColor
        }
    }

    Rectangle {
        id: loadingView
        color: theme.primaryBackground

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 20

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Spinner {
                    width: 30
                }

                Text {
                    text: qsTr("Отправка запроса")
                    color: theme.textColor
                    font.pixelSize: 16
                }
            }
        }
    }

    ResponseView {
        id: responseView
    }

}
