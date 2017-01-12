import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Particles 2.0
import QtQml.Models 2.2



ApplicationWindow {
    id: root
    width: 940
    height: 600
    visible: true
    color: "#F5FFFA"
    title: qsTr("LECTIONS")

    menuBar: MenuBar {

        Menu {
            title: qsTr("File")

           /* MenuBarStyle {
                background: Rectangle {color: "#008080"}
            }*/

            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }

    }

    TreeView {
    id: treeView
    alternatingRowColors : false
    //backgroundVisible : false
    x: 3
    y: 3
    /* Mogno dlya krasoti
    contentFooter : Component {
                  Rectangle {
                    color: "#008B8B"
                    width: parent.width
                    height: 25
                }
            }
    contentHeader: Component { // zachem!?
          Rectangle {
            color: "#008B8B"
            width: parent.width
            height: 25
        }
    }*/
    //headerVisible : false
    itemDelegate: Item {
        Text {
            anchors.verticalCenter: parent.verticalCenter
            color: "#191970"
            elide: styleData.elideMode
            text: styleData.value
             }
    }
    //sortIndicatorColumn : 1


    width: parent.width/3.4 //ширина
    height: parent.height - 6
    model: mymodel


    TableViewColumn{
    title:"Choose the lection"
    role: "display"
    width:500 //появляется прокрутка
    /*delegate: Rectangle {
        color: "yellow"
    }*/
    }

    }

   /* TableViewStyle {

        alternateBackgroundColor : "red"
        backgroundColor : "yellow"
        highlightedTextColor : "blue"
        textColor : "black"

    }*/





    }























/*ApplicationWindow {
    id: root
    width: 640
    height: 400
    visible: true
    color: "#F5FFFA"
    title: qsTr("LECTIONS")
    /*
        menuBar: MenuBar {
           style: MenuBarStyle {
           background: Rectangle { color: "#008080"}
    }
           Menu {
               title: qsTr("File")

               MenuItem {
                   text: qsTr("&Choose semester")
                   onTriggered: console.log("Open action triggered");
               }
               MenuItem {
                   text: qsTr("Exit")
                   onTriggered: Qt.quit();
               }
           }
        }

}*/














