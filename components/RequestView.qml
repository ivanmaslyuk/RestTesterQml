import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import "."

SplitView {
    id: control

    handle: Rectangle {
        implicitWidth: 2

        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            color: "#EEEEEE"
            width: 1
        }
    }

    SplitView {
        id: requestPane
        orientation: Qt.Vertical
        SplitView.minimumWidth: 380
        SplitView.fillWidth: true
        clip: true

        handle: Rectangle {
            color: "#EEEEEE"
            implicitHeight: 1
        }

        RequestInfoView {
            id: requestInfoView
            SplitView.minimumHeight: implicitHeight
        }

        ResponseView {
            id: responseView
            SplitView.minimumHeight: 40
            SplitView.fillHeight: true
        }
    }

    DocumentationView {
        id: documentationPane
        visible: requestInfoView.documentationShown
        SplitView.minimumWidth: 200
    }
}
