import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Particles 2.0
import QtQml.Models 2.2
import QtQuick.Dialogs 1.2




ApplicationWindow {
    id: root
    width: 760
    height: 600
    visible: true
    color: "#F5F5F5"

    title: qsTr("LECTIONS")

    menuBar: MenuBar {

        style: MenuBarStyle {

             background: Component {
                Rectangle {
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#696969" }
                        GradientStop { position: 1.0; color: "#C0C0C0" }
                }
                }

            }
        }



        Menu {
            title: qsTr("File")
            style: MenuStyle {
                frame:  Rectangle {color: "#000000"}
                itemDelegate.background : Rectangle {color: "#C0C0C0"}
            }


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
            style: MenuStyle {
                frame:  Rectangle {color: "#000000"}
                itemDelegate.background : Rectangle {color: "#C0C0C0"}
            }


            MenuItem {
                text: qsTr("&Add the semester")
                onTriggered: messageDialog.show(qsTr("Button 1 pressed"))
            }
            MenuItem {
                text: qsTr("&Add the subject")
                onTriggered: messageDialog.show(qsTr("Button 2 pressed"))
            }
            MenuItem {
                text: qsTr("&Add the lection page")
                onTriggered: messageDialog.show(qsTr("Button 3 pressed"))
            }
        }

    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

    TreeView {
    id: treeView
    //backgroundVisible : false
    x: 3
    y: 3
    headerVisible : false
    width: parent.width/3 //ширина
    height: parent.height - 6
    model: mymodel

    style: TreeViewStyle {
              alternateBackgroundColor : "#DCDCDC" // vipadaet
              backgroundColor : "#DCDCDC" // osnovnoi
              textColor: "#000000"

    }

    MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: if (mouse.button === Qt.LeftButton)
                       {
                       var index_1 = parent.indexAt(mouse.x, mouse.y);
                       if (index_1.valid)
                       {
                           if(mymodel.data(index_1,1))
                           {
                               im.source = mymodel.data(index_1,1);

                           }
                           else
                           {
                               parent.isExpanded(index_1) ? parent.collapse(index_1) : parent.expand(index_1);

                           }

                       }
        }
    }

    Image
     {
         id: im
         x: 240
         y: 10
         width: 550
         height: 550
         fillMode: Image.PreserveAspectFit
     }




    TableViewColumn{
    role: "display"
    width:500 //появляется прокрутка
        }
}
}














