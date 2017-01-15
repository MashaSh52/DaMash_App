import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Particles 2.0
import QtQml.Models 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.0
import QtWebKit 3.0

ApplicationWindow {
    id: child
    width: 1200
    height:600
    visibility: Window.FullScreen
    title: qsTr("IMAGE")


    menuBar: MenuBar {
        style: MenuBarStyle { background: Rectangle {color: "black"}}

        Menu {
            title: qsTr("File")

            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }


    Image
     {
         id: image
         anchors.fill: parent
         width: parent.width
         height: parent.height
         source: dbModel.data(dbModel.getTransitIndex(),1)
         fillMode: Image.PreserveAspectFit
     }

}
