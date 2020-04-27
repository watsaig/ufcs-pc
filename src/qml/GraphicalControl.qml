import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.example.ufcs 1.0 // for the Style singleton

Item {
    id: control

    // When True: buttons are disabled; user can edit labels
    property bool editingMode : false

    property int minimumWidth : inputValveGrid.implicitWidth + 2*Style.view.margin
    property int maximumWidth: 1000

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Style.view.margin
        anchors.horizontalCenter: parent.horizontalCenter

        GraphicalChipControl {
            id: graphicalChipControl
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            Layout.maximumWidth: control.maximumWidth

            enabled: !editingMode
        }

        Column {
            Layout.fillWidth: true
            Layout.maximumWidth: control.maximumWidth
            Layout.alignment: Qt.AlignHCenter

            Label {
                text: qsTr("Multiplexer")
                font.pointSize: Style.title.fontSize
                padding: Style.title.padding
                leftPadding: Style.title.paddingLeft
                Layout.alignment: Qt.AlignTop
            }

            RowLayout {
                id: multiplexerButtons
                anchors.left: parent.left
                anchors.right: parent.right

                MultiplexerButton { id: b1; number: 1; resizeProportionally: false}
                MultiplexerButton { number: 2; resizeProportionally: false}
                MultiplexerButton { number: 3; resizeProportionally: false}
                MultiplexerButton { number: 4; resizeProportionally: false}
                MultiplexerButton { number: 5; resizeProportionally: false}
                MultiplexerButton { number: 6; resizeProportionally: false}
                MultiplexerButton { number: 7; resizeProportionally: false}
                MultiplexerButton { number: 8; resizeProportionally: false}
            }


            RowLayout {
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    text: qsTr("Inputs")
                    Layout.fillWidth: true
                    font.pointSize: Style.title.fontSize
                    padding: Style.title.padding
                    leftPadding: Style.title.paddingLeft
                }

                Button {
                    id: editingModeSwitch
                    text: control.editingMode ? qsTr("Done") : qsTr("Edit labels")
                    flat: true
                    Layout.alignment: Qt.AlignRight
                    onClicked: control.editingMode = !control.editingMode
                }
            }


            GridLayout {
                id: inputValveGrid
                anchors.left: parent.left
                anchors.right: parent.right
                columns: 6

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch
                    valveNumber: 19
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch1
                    valveNumber: 20
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch2
                    valveNumber: 21
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch3
                    valveNumber: 22
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch4
                    valveNumber: 23
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch5
                    valveNumber: 24
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch6
                    valveNumber: 25
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch7
                    valveNumber: 26
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch8
                    valveNumber: 27
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch9
                    valveNumber: 28
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch10
                    valveNumber: 29
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch11
                    valveNumber: 30
                    editable: editingMode
                }

                GraphicalLabeledValveSwitch {
                    id: graphicalLabeledValveSwitch12
                    valveNumber: 31
                    editable: editingMode
                }
            }

            PressureControlPane {
                id: pressureControlPane
                enabled: !editingMode

                anchors.left: parent.left
                anchors.right: parent.right
            }
        }
    }
    Component.onCompleted: {
        // Bit of a hack but I don't know how to do this from mainWindow
        mainWindow.minimumWidth = Qt.binding(function() { return minimumWidth})
    }
}







/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:14;flowX:"-324";flowY:161}
}
##^##*/
