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

    Button
    {
       id:button1
        x: 5
        y: 3
        style: ButtonStyle {
            label: Text {
                text: qsTr("Add New Term")
                color: control.pressed ? "#696969" : "#FFFFFF"
            }
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 25
                color: control.pressed ? "#DCDCDC" : "#696969"
                border.color: "#000000"
                border.width: 1
                radius: 3
            }
        }
        onClicked: popup1.open()//adding1.visible = true


    }

    TreeView {
    id: treeView
    //backgroundVisible : false
    x: 5
    y: 36
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
    width:2*treeView.width
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
                               dbModel.setTransitIndex(iLeft);
                               printButton.visible = true
                               imProcButton.visible = true

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


    GridLayout {
        rowSpacing: 2
        y: 3
        x: parent.width - 110

// PRINT BUTTON
    Button
    {
       id:printButton
       visible: false
        style: ButtonStyle {
            label: Text {
                text: qsTr("Print")
                color: control.pressed ? "#696969" : "#FFFFFF"
            }
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 25
                color: control.pressed ? "#DCDCDC" : "#696969"
                border.color: "#000000"
                border.width: 1
                radius: 3
            }
        }
        onClicked: popup_print.open() // + printFunction
    }

// PROCESSING BUTTON
    Button
    {
       id:imProcButton
       visible: false
        Layout.row: 1
        style: ButtonStyle {
            label: Text {
                text: qsTr("Processing")
                color: control.pressed ? "#696969" : "#FFFFFF"
            }
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 25
                color: control.pressed ? "#DCDCDC" : "#696969"
                border.color: "#000000"
                border.width: 1
                radius: 3
            }
        }
        onClicked: popup_filters.open()


    }




}
// PRINTING MESSAGE
    Popup {
        id: popup_print
        x:root.width/2
        y:root.height/4
        width: 200
        height: 100
        background: Rectangle { color: "#696969" }
        Label {
                    text: qsTr("Printing. Please,wait...")
                }

    }



// IMAGE PROCESSING CheckBox

    Popup {
        id: popup_filters
        x:root.width/2
        y:root.height/4
        visible: false
        background: Rectangle { color: "#696969" }
        ColumnLayout{
        Label {
                    text: qsTr("Type of processing")
                }
        Button {
            id: rotate_button
          //  text: qsTr("Rotate")
            style: ButtonStyle {
                       label: Text {
                           text: qsTr("Rotate")
                           color: control.pressed ? "#696969" : "#FFFFFF"
                       }
                       background: Rectangle {
                           implicitWidth: 100
                           implicitHeight: 25
                           color: control.pressed ? "#DCDCDC" : "#696969"
                           border.color: "#000000"
                           border.width: 1
                           radius: 3
                       }
                   }
            onClicked: { popup_rotate.open()
                popup_filters.close()
            }

        }
        Button {
            id: crop_button
          //  text: qsTr("Crop")
            style: ButtonStyle {
                label: Text {
                    text: qsTr("Crop")
                    color: control.pressed ? "#696969" : "#FFFFFF"
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    color: control.pressed ? "#DCDCDC" : "#696969"
                    border.color: "#000000"
                    border.width: 1
                    radius: 3
                }
            }
            onClicked: {
                popup_crop.open()
                popup_filters.close()
            }
        }
        Button {
            id: baw_button
            //text: qsTr("Black and White")
            style: ButtonStyle {
                label: Text {
                    text: qsTr("Black and White")
                    color: control.pressed ? "#696969" : "#FFFFFF"
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    color: control.pressed ? "#DCDCDC" : "#696969"
                    border.color: "#000000"
                    border.width: 1
                    radius: 3
                }
            }
            onClicked:  { new_im.source = dbModel.makeBlackAndWhiteImage(dbModel.getTransitIndex());

                popup_filters.close();}
        }

    }
}

// SHOW THE PROCESSED IMAGE
    Image
     {
         id: new_im
         x: parent.width/3
         width: root.width
         height: parent.height
         fillMode: Image.PreserveAspectFit
     }

// ROTATION
    Popup {
        id: popup_rotate
        x:root.width/2
        y:root.height/4
        visible: false
        background: Rectangle { color: "#696969" }

        ColumnLayout{

            spacing: 10
            anchors.centerIn: parent

        Label {
                    text: qsTr("PARAMETER")
                }

        TextField {
            id: angle
            placeholderText: qsTr("angle = ")
            width: 250
            textColor: "#696969"
        }

        Button {
            id:button_close_1
             style: ButtonStyle {
                 label: Text {
                     text: qsTr("Rotate")
                     color: control.pressed ? "#696969" : "#FFFFFF"
                 }
                 background: Rectangle {
                     color: control.pressed ? "#DCDCDC" : "#696969"
                     border.color: "#000000"
                     border.width: 0.5
                     radius: 2
                 }
             }
             onClicked: {
                 new_im.source = dbModel.rotateImage(dbModel.getTransitIndex(), angle);
                 popup_rotate.close()
                              }
               }
        }
    }

// CROP
    Popup {
        id: popup_crop
        x:root.width/2
        y:root.height/4
        visible: false
        background: Rectangle { color: "#696969" }
        ColumnLayout{
        Label {
                    text: qsTr("PARAMETERS")
                }
        TextField {
            id: x1
            placeholderText: qsTr("x1 = ")
            width: 250
            textColor: "#696969"
        }
        TextField {
            id: x2
            placeholderText: qsTr("x2 = ")
            width: 250
            textColor: "#696969"
        }
        TextField {
            id: y1
            placeholderText: qsTr("y1 = ")
            width: 250
            textColor: "#696969"
        }
        TextField {
            id: y2
            placeholderText: qsTr("y2 = ")
            width: 250
            textColor: "#696969"
        }
        Button {
            id:button_close_2
             style: ButtonStyle {
                 label: Text {
                     text: qsTr("Crop")
                     color: control.pressed ? "#696969" : "#FFFFFF"
                 }
                 background: Rectangle {
                     color: control.pressed ? "#DCDCDC" : "#696969"
                     border.color: "#000000"
                     border.width: 0.5
                     radius: 2
                 }
             }
             onClicked: {
                 new_im.source = dbModel.cropImage(dbModel.getTransitIndex(),x1, y1, x2, y2);
                 popup_crop.close()
                              }
               }

        }
    }
  /*  ButtonStyle {
        id: button_style
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    color: control.pressed ? "#DCDCDC" : "#696969"
                    border.color: "#000000"
                    border.width: 1
                    radius: 3
                }
            }
*/

}


