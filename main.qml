import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0


ApplicationWindow {
    id: root
    visible: true
    width: 500
    height: 360
    title: qsTr("Лекции")

    toolBar: ToolBar{
        Button{
            anchors.centerIn: parent
            text:"Push me"
            onClicked: root.color = "khaki"
        }
    }
}


