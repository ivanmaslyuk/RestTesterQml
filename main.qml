import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import "components"

Window {
    width: 500
    height: 600
    visible: true
    title: "REST Tester"

    SplitView {
        orientation: Qt.Vertical
        anchors.fill: parent

        handle: Rectangle {
            color: "#EEEEEE"
            implicitHeight: 1
        }

        RequestInfoView {
            SplitView.minimumHeight: implicitHeight
        }

        ResponseView {
            SplitView.minimumHeight: 20

            bodyText: "{\n  \"key\": \":value\",\n  {\n    \"key2\": \"this value is very long, isn't it?\"\n  }\n}"
            headersModel: ListModel {
                ListElement {
                    name: "Status"
                    value: "200"
                    isChecked: true
                }
                ListElement {
                    name: "Content-Length"
                    value: "22"
                    isChecked: false
                }
                ListElement {
                    name: "Content-Type"
                    value: "application/json"
                    isChecked: true
                }
            }
        }
    }
}
