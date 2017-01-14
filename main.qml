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




ApplicationWindow {
    id: root
    width: 860
    height: 600
    visible: true
    color: "#F5F5F5"//"#F5F5F5"

    title: qsTr("LECTIONS")

    menuBar: MenuBar {
             style: MenuBarStyle { background:  Rectangle { color: "#696969" } }


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

    }

    Button
    {
       id:button1
        x: 3
        y: 20
        style: ButtonStyle {
            label: Text {
                text: qsTr("Add New Term")
                color: control.pressed ? "#696969" : "#FFFFFF"
            }
            background: Rectangle {
                color: control.pressed ? "#DCDCDC" : "#696969"
                border.color: "#696969"
                border.width: 0.5
                radius: 2
            }
        }
        onClicked: adding1.visible = true


    }

    Rectangle {
        id:adding1
        visible: false
        anchors.centerIn: parent
        width: 300
        height: 100
        color: "#696969"

     /*   Material.theme: Material.Light
        Material.primary: Material.BlueGray
        Material.accent: Material.Teal*/

        Column {
               spacing: 10
               anchors.centerIn: parent

        TextField {
            id: textField1
            objectName: "textField1"
            placeholderText: qsTr("Enter the Term")
            width: 250
            textColor: "#696969"
        }

        Button {
            id:button2
             style: ButtonStyle {
                 label: Text {
                     text: qsTr("Add")
                     color: control.pressed ? "#696969" : "#FFFFFF"
                 }
                 background: Rectangle {
                     color: control.pressed ? "#DCDCDC" : "#696969"
                     border.color: "#696969"
                     border.width: 0.5
                     radius: 2
                 }
             }
             onClicked: console.log("It works");
               }
        }


    }

   /* Popup {
        id: popup
        x: 100
        y: 100
        width: 200
        height: 300
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    }
*/
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
    y: 53
    headerVisible : false
    width: parent.width/3.5 //ширина
    height: parent.height - 60
    model: dbModel

    style: TreeViewStyle {
              alternateBackgroundColor : "#696969" // vipadaet
              backgroundColor : "#696969" // osnovnoi
              textColor: "#FFFFFF"

    }

    MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: if (mouse.button === Qt.LeftButton)
                       {
                       var ind = parent.indexAt(mouse.x, mouse.y);
                       if (ind.valid)
                       {
                           if(dbModel.data(ind,1))
                           {
                               im.source = dbModel.data(ind,1);

                           }
                           else
                           {// parent.isExpanded(ind) ? parent.collapse(ind) : parent.expand(ind);
                               if (parent.isExpanded(ind))
                                    {

                                   var children = dbModel.getChildrenIndexesOfItem(ind);
                                         for (var i = children.length-1; i >= 0; i--)
                                           {

                                              if (parent.isExpanded(children[i]))
                                              {
                                                parent.collapse(children[i])
                                              }
                                     }
                                  // parent.collapse(ind)
                               }
                               else { parent.expand(ind); }
                           }

                       }
        }
    }

    Image
     {
         id: im
         x: parent.width
         y: -20
         width: 500
         height: 530
         fillMode: Image.PreserveAspectFit
     }


    TableViewColumn{
    role: "display"
    width:500 //появляется прокрутка
        }
}
}


