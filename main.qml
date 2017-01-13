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

        style: MenuBarStyle {
            background: Rectangle {color: "#008080"}
        }

        Menu {
            title: qsTr("File")


            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
            MenuItem {
                text: qsTr("Print")
                //onTriggered:  MessageDialog.show(qsTr("Open action triggered")); v chem raznica?
                action: console.log("Open action triggered");
            }
        }
        Menu {
            title: qsTr("Add..")


            MenuItem {
                text: qsTr("&Add the semester")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("&Add the subject")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("&Add the lection page")
                onTriggered: console.log("Open action triggered");
            }
        }

    }

    TreeView {
    id: treeView
    alternatingRowColors : false
    //backgroundVisible : false
    x: 3
    y: 3


    width: parent.width/3.4 //ширина
    height: parent.height - 6
    model: mymodel

    style: TreeViewStyle {
              alternateBackgroundColor : "#BDB76B" // vipadaet
              backgroundColor : "#556B2F" // osnovnoi
              textColor: "#7CFC00"
              highlightedTextColor : "#FF6347"
              headerDelegate: Component {
                  Rectangle {
                    border.color: "black"
                    border.width: 1
                    width: parent.width
                    height: 30
                    gradient: Gradient {
                            GradientStop { position: 0.0; color: "lightsteelblue" }
                            GradientStop { position: 1.0; color: "blue" }
                    }

            }
          }
    }

    MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: if (mouse.button === Qt.LeftButton)
                       {
                       var index_1 = parent.indexAt(mouse.x, mouse.y);
                       if (index_1.valid)
                       { parent.isExpanded(index_1) ? parent.collapse(index_1) : parent.expand(index_1);}
                       }
        }


    TableViewColumn{
    title:"Choose the lection"
    role: "display"
    width:500 //появляется прокрутка

        }



}

}






// in treeview

//headerVisible : false
/*  itemDelegate: Item {
    Text {
        anchors.verticalCenter: parent.verticalCenter
        color: "#191970"
        elide: styleData.elideMode
        text: styleData.value
         }
}*/
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
















