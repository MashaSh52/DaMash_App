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
//import QtQuick 2.6
//import QtQuick.Controls 2.1

import QtQuick.Layouts 1.3
import QtQuick.Window 2.0


ApplicationWindow {
    id: root
    width: 860
    height: 600
    visible: true
    color: "#F5F5F5"

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
        onClicked: popup1.open()//adding1.visible = true


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


    Image
     {
         id: im
         x: parent.width/3
         width: root.width
         height: parent.height
         fillMode: Image.PreserveAspectFit
     }


    TableViewColumn{
    role: "display"
    width:500 //появляется прокрутка
        }

    MouseArea {
            id: mouseArea
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if(mouse.button === Qt.RightButton)

                {
                            var iRight = parent.indexAt(mouse.x, mouse.y);
                            if (iRight.valid)  {

                                dbModel.setTransitIndex(iRight);
                                var j = dbModel.whatElementIsIt(iRight)
                                if(j === 1) { menu1.popup()}
                                if(j === 2) { menu2.popup()}
                                if(j === 3) { menu3.popup()}

                                 }
                }
                if (mouse.button === Qt.LeftButton)
                       {
                       var iLeft = parent.indexAt(mouse.x, mouse.y);
                       if (iLeft.valid)
                       {

                           if(dbModel.data(iLeft,1))
                           {
                               im.source = dbModel.data(iLeft,1);

                           }
                           else
                           {
                               if (parent.isExpanded(iLeft))
                                    {

                                   var children = dbModel.getChildrenIndexesOfItem(iLeft);
                                         for (var i = children.length-1; i >= 0; i--)
                                           {

                                              if (parent.isExpanded(children[i]))
                                              {
                                                parent.collapse(children[i])
                                              }
                                     }
                                  // parent.collapse(iLeft)
                               }
                               else { parent.expand(iLeft); }
                           }

                       }
        }


    }
    }

    Menu {
    id: menu1
    //visible: false
    title: qsTr("Add")

    MenuItem {
        id:courseAdd
        text: qsTr("&Add the Course")
        onTriggered:  popup2.open()
               }

    MenuItem {
        text: qsTr("&Delete")
        onTriggered:  dbModel.deleteElement(dbModel.getTransitIndex());

             }
    }

    Menu {
    id: menu2
    //visible: false
    title: qsTr("Add")

    MenuItem {
        id:imageAdd
        text: qsTr("&Add the Image")
        onTriggered:  popup3.open()
               }

    MenuItem {
        text: qsTr("&Delete")
        onTriggered:  dbModel.deleteElement(dbModel.getTransitIndex());

             }
    }

    Menu {
    id: menu3
    //visible: false
    title: qsTr("Add")

    MenuItem {
        text: qsTr("&Delete")
        onTriggered:  dbModel.deleteElement(dbModel.getTransitIndex());

             }
    }


// FOR TERM ADDING

    Popup {
        id: popup1
        x:root.width/2
        y:root.height/3.5
        width: 300
        height: 300
        background: Rectangle { color: "#696969" }
       // modal: true
       // focus: true
        //clip: true

        contentItem : Item {

            ColumnLayout {
                        spacing: 10
                        anchors.centerIn: parent

                    TextField {
                        id: textField1
                        objectName: "textField1"
                        placeholderText: qsTr("Term")
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
                         onClicked: {
                             dbModel.addNewTerm(textField1.text);
                             popup1.close()
                                          }
                           }
                    }
        }


    }



// FOR COURSE ADDING
    Popup {
        id: popup2
        x:root.width/2
        y:root.height/3.5
        width: 300
        height: 300
        background: Rectangle { color: "#696969" }

        contentItem : Item {

            ColumnLayout {
                        spacing: 10
                        anchors.centerIn: parent

                    TextField {
                        id: textField2
                        objectName: "textField2"
                        placeholderText: qsTr("Course")
                        width: 250
                        textColor: "#696969"
                    }

                    Button {
                        id:button3
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
                         onClicked: {
                             dbModel.addNewCourse(dbModel.getTransitIndex(),textField2.text);
                             popup2.close()
                                          }
                           }
                    }
        }


    }


// FOR IMAGE ADDING
    Popup {
        id: popup3
        x:root.width/2
        y:root.height/3.5
        width: 800
        height: 600
        background: Rectangle { color: "#696969" }

        contentItem : Item {

            ColumnLayout {
                        spacing: 10
                        anchors.centerIn: parent

                    TextField {
                        id: textField3
                        placeholderText: qsTr("Path to Image")
                        width: 250
                        textColor: "#696969"
                    }
                    TextField {
                        id: textField4
                        placeholderText: qsTr("Comments")
                        width: 250
                        textColor: "#696969"
                    }
                    TextField {
                        id: textField5
                        placeholderText: qsTr("Tags")
                        width: 250
                        textColor: "#696969"
                    }

                    Button {
                        id:button4
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
                         onClicked: {
                             dbModel.addNewImage(dbModel.getTransitIndex(), textField3.text, textField4.text, textField5.text)
                             popup3.close()
                                          }
                           }
                    }
        }


    }














}
}







/*
    Rectangle {
        id:adding1
        visible: false
        anchors.centerIn: parent
        width: 300
        height: 100
        color: "#696969"
      //  antialiasing: false

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
             onClicked: {
                 dbModel.addNewTerm(textField1.text);
                 adding1.visible = false
                              }
               }
        }


    }*/

