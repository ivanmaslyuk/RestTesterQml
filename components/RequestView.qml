import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import "."

SplitView {
    handle: Rectangle {
        color: "#EEEEEE"
        implicitWidth: 1
    }

    SplitView {
        orientation: Qt.Vertical
        SplitView.minimumWidth: 420

        handle: Rectangle {
            color: "#EEEEEE"
            implicitHeight: 1
        }

        RequestInfoView {
            SplitView.minimumHeight: implicitHeight
        }

        ResponseView {
            SplitView.minimumHeight: 40

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

    DocumentationView {
        SplitView.minimumWidth: 200
    }
}
