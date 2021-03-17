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
            currentIndex = 0
        }
    }

    ResponseView {
        id: responseView
    }

    Rectangle {
        id: loadingView

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
                    font.pixelSize: 16
                }
            }
        }
    }

}
