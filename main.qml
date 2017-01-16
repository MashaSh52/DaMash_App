import QtQuick 2.7
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
    id: root
    width: 860
    height: 600
    minimumWidth: 800
    minimumHeight: 500
    visible: true
    style: ApplicationWindowStyle { background: Rectangle {color: "#FFFFFF"}}
    title: qsTr("LECTIONS")

    Button
    {
       id:button1
        x: 5
        y: 3
        style: ButtonStyle {
            label: Text {
                text: qsTr("Add New Term")
                color: control.pressed ? "black" : "#696969"
                font.bold: true
            }
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 25
                color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                border.width: 3
            }
        }

        onClicked: popup1.open()


    }

TreeView {
    id: treeView
    x: 5
    y: 36
    headerVisible : false
    width: parent.width/3.5
    height: parent.height - 100
    model: dbModel

    style: TreeViewStyle {

              activateItemOnSingleClick : true
              alternateBackgroundColor : "#FFFFFF"
              backgroundColor : "#FFFFFF"
              textColor:"#696969"
              highlightedTextColor :"#D3D3D3"

    }


    Image
     {
         id: im
         x: 4*parent.width/3
         width: root.width/2
         height: parent.height
         fillMode: Image.PreserveAspectFit
         MouseArea {
                 id: imMouseArea
                 anchors.fill: parent
                 acceptedButtons: Qt.LeftButton | Qt.RightButton
                 onClicked: {
                     if(mouse.button === Qt.RightButton)

                     {
                         menuFullScreen.popup()

                      }
         }
}
}


// MENU FOR IMAGE

    Menu {
    id: menuFullScreen
     title: qsTr("Open")

    MenuItem {
        text: qsTr("&Full Screen")
        onTriggered:  {

                        var component = Qt.createComponent("Child.qml")
                        var window    = component.createObject(root)
                        window.show()
}

             }
    MenuItem {
        text: qsTr("&Add Comment")
        onTriggered:  popup_Comment.open()

             }
    MenuItem {
        text: qsTr("&Add Tag")
        onTriggered:  popup_Tags.open()

             }
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
                               dbModel.setTransitURL(im.source);
                               dbModel.setTransitIndex(iLeft);

                              // appears all buttons connected with the Image
                               printButton.visible = true
                               imProcButton.visible = true
                               setComm.visible = true
                               setTag.visible = true

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
                               }
                               else { parent.expand(iLeft); }
                           }

                       }
        }


    }
    }

    Menu {
    id: menu1
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
                                color: control.pressed ? "black" : "#696969"
                                font.bold: true
                            }
                            background: Rectangle {
                                implicitWidth: 100
                                implicitHeight: 25
                                color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                                border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                                border.width: 3
                            }
                        }
                         onClicked: {
                             if(dbModel.addNewTerm(textField1.text))
                              {
                                popup_error.open()
                              }
                             popup1.close()
                                          }
                           }
                    }
        }


    }

// ERROR MESSAGE
    Popup {
        id: popup_error
        x:root.width/2
        y:root.height/4
        width: 200
        height: 100

        ColumnLayout {
        Label {
                    text: qsTr("Error!The parameter is entered or already exist!")
                    color: "red"
                }
        Button
        {
           id:sadOk
           Layout.column:4
           style: ButtonStyle {
               label: Text {
                   text: qsTr("   OK  =(    ")
                   color: control.pressed ? "black" : "#696969"
                   font.bold: true
               }
               background: Rectangle {
                   implicitWidth: 100
                   implicitHeight: 25
                   color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                   border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                   border.width: 3
               }
           }
            onClicked: popup_error.close()
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
                                color: control.pressed ? "black" : "#696969"
                                font.bold: true
                            }
                            background: Rectangle {
                                implicitWidth: 100
                                implicitHeight: 25
                                color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                                border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                                border.width: 3
                            }
                        }

                         onClicked: {
                             if(dbModel.addNewCourse(dbModel.getTransitIndex(),textField2.text))
                              {

                                 popup_error.open()
                              }
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
                                color: control.pressed ? "black" : "#696969"
                                font.bold: true
                            }
                            background: Rectangle {
                                implicitWidth: 100
                                implicitHeight: 25
                                color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                                border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                                border.width: 3
                            }
                        }
                         onClicked: {
                             if(dbModel.addNewImage(dbModel.getTransitIndex(), textField3.text, textField4.text, textField5.text))
                             {
                                   popup_error.open()
                             }

                             popup3.close()
                                          }
                           }

                    }
        }
    }
}


    GridLayout {
        columnSpacing: 4
        y: 3
        x: parent.width - 440

// PRINT BUTTON
    Button
    {
       id:printButton
       visible: false
       Layout.column:1
       style: ButtonStyle {
           label: Text {
               text: qsTr("Print")
               color: control.pressed ? "black" : "#696969"
               font.bold: true
           }
           background: Rectangle {
               implicitWidth: 100
               implicitHeight: 25
               color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
               border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
               border.width: 3
           }
       }
        onClicked: popup_print.open() // + printFunction
    }

// PROCESSING BUTTON
    Button
    {
       id:imProcButton
       visible: false
       Layout.column:2
       style: ButtonStyle {
           label: Text {
               text: qsTr("Processing")
               color: control.pressed ? "black" : "#696969"
               font.bold: true
           }
           background: Rectangle {
               implicitWidth: 100
               implicitHeight: 25
               color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
               border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
               border.width: 3
           }
       }
        onClicked: popup_filters.open()
    }

// SET COMMENT BUTTON
            Button
            {
               id:setComm
               Layout.column:3
               visible: false
               style: ButtonStyle {
                   label: Text {
                       text: qsTr("Set Comment")
                       color: control.pressed ? "black" : "#696969"
                       font.bold: true
                   }
                   background: Rectangle {
                       implicitWidth: 100
                       implicitHeight: 25
                       color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                       border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                       border.width: 3
                   }
               }
                onClicked: popup_Comment.open()


    }

// SET TAG BUTTON
            Button
            {
               id:setTag
               Layout.column:4
               visible: false
               style: ButtonStyle {
                   label: Text {
                       text: qsTr("Set Tag")
                       color: control.pressed ? "black" : "#696969"
                       font.bold: true
                   }
                   background: Rectangle {
                       implicitWidth: 100
                       implicitHeight: 25
                       color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                       border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                       border.width: 3
                   }
               }
                onClicked: popup_Tags.open()

    }
    }

// SET TAGS POPUP

Popup {
                id: popup_Tags
                x:root.width/2
                y:root.height/4
                visible: false

                ColumnLayout{

                    spacing: 10
                    anchors.centerIn: parent


                TextField {
                    id: tag
                    placeholderText: qsTr("Tag")
                    width: 450
                    textColor: "#696969"
                }

                Button {
                    id:b1
                    style: ButtonStyle {
                        label: Text {
                            text: qsTr("Enter")
                            color: control.pressed ? "black" : "#696969"
                            font.bold: true
                        }
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 25
                            color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                            border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                            border.width: 3
                        }
                    }
                     onClicked: {
                         dbModel.setTags(dbModel.getTransitIndex(), tag.text);
                         popup_Tags.close()
                                      }
                       }
                }
            }

// SET COMMENT POPUP
            Popup {
                id: popup_Comment
                x:root.width/2
                y:root.height/4
                visible: false

                ColumnLayout{

                    spacing: 10
                    anchors.centerIn: parent


                TextField {
                    id: comment
                    placeholderText: qsTr("Comment")
                    width: 450
                    textColor: "#696969"
                }

                Button {
                    id:b2
                    style: ButtonStyle {
                        label: Text {
                            text: qsTr("Enter")
                            color: control.pressed ? "black" : "#696969"
                            font.bold: true
                        }
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 25
                            color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                            border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                            border.width: 3
                        }
                    }
                     onClicked: {
                         dbModel.setComment(dbModel.getTransitIndex(), comment.text);
                         popup_Comment.close()
                                      }
                       }
                }
            }

// PRINTING MESSAGE

    Popup {
        id: popup_print
        x:root.width/2
        y:root.height/4
        width: 200
        height: 100
        Label {
                    text: qsTr("Printing. Please,wait...")
                    color: "#696969"
                }

    }


// IMAGE PROCESSING

    Popup {
        id: popup_filters
        x:root.width/2
        y:root.height/4
        visible: false

        ColumnLayout{
        Label {
                    text: qsTr("TYPE OF PROCESSING")
                    color: "#696969"
                }
        Button {
            id: rotate_button
            style: ButtonStyle {
                label: Text {
                    text: qsTr("Rotate")
                    color: control.pressed ? "black" : "#696969"
                    font.bold: true
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                    border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                    border.width: 3
                }
            }
            onClicked: { popup_rotate.open()
                popup_filters.close()
            }

        }
        Button {
            id: crop_button
            style: ButtonStyle {
                label: Text {
                    text: qsTr("Crop")
                    color: control.pressed ? "black" : "#696969"
                    font.bold: true
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                    border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                    border.width: 3
                }
            }
            onClicked: {
                popup_crop.open()
                popup_filters.close()
            }
        }
        Button {
            id: baw_button
            style: ButtonStyle {
                label: Text {
                    text: qsTr("Black and White")
                    color: control.pressed ? "black" : "#696969"
                    font.bold: true
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                    border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                    border.width: 3
                }
            }
            onClicked:  { im.source = dbModel.makeBlackAndWhiteImage(im.source.toString().slice(7),"/home/skt/im_baw.jpg");
                dbModel.setTransitURL(im.source);
                popup_filters.close();}
        }

    }
}


// ROTATION
    Popup {
        id: popup_rotate
        x:root.width/2
        y:root.height/4
        visible: false

        ColumnLayout{

            spacing: 10
            anchors.centerIn: parent

        Label {
                    text: qsTr("PARAMETER")
                    color: "#696969"
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
                    color: control.pressed ? "black" : "#696969"
                    font.bold: true
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                    border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                    border.width: 3
                }
            }
             onClicked: {
                 im.source = dbModel.rotateImage(im.source.toString().slice(7),"/home/skt/im_rotate.jpg", angle.text);
                 dbModel.setTransitURL(im.source);
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

        ColumnLayout{
        Label {
                    text: qsTr("PARAMETERS")
                    color: "#696969"
                }
        TextField {
            id: x1
            placeholderText: qsTr("x1 = ")
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
            id: x2
            placeholderText: qsTr("x2 = ")
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
                    color: control.pressed ? "black" : "#696969"
                    font.bold: true
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    color: control.pressed ? "#D3D3D3" : (control.hovered ? "#F5F5F5" :"#FFFFFF")
                    border.color: control.pressed ? "#696969" : (control.hovered ? "#C0C0C0" :"#DCDCDC")
                    border.width: 3
                }
            }
             onClicked: {

                 im.source = dbModel.cropImage(im.source.toString().slice(7),"/home/skt/im_crop.jpg",x1.text, y1.text, x2.text, y2.text);
                 dbModel.setTransitURL(im.source);
                 popup_crop.close()
                              }
               }

        }
    }

}


