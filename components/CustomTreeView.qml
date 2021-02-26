import QtQuick 2.0
import QtQuick.Controls 1.6

TreeView {
    id: control
    headerVisible: false
    model: myTreeModel
    TableViewColumn {
        title: "Name"
        role: "display"
    }

    Connections {
        target: control
        onActivated: function (index) {
            console.log(index)
        }
    }
}
