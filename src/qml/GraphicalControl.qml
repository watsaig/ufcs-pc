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

            enabled: !editingMode && Backend.connectionStatus == "Connected"
        }

        Column {
            Layout.fillWidth: true
            Layout.maximumWidth: control.maximumWidth
            Layout.alignment: Qt.AlignHCenter

            Label {
                text: qsTr("Multiplexer")
                font.pointSize: Style.heading1.fontSize
                padding: Style.heading1.padding
                leftPadding: Style.heading1.paddingLeft
                Layout.alignment: Qt.AlignTop
            }

            MultiplexerControl {
                id: muxControl
                anchors.left: parent.left
                anchors.right: parent.right

                valves: [13, 14, 15, 16, 17, 18]
                muxModel: ListModel {
                    ListElement { label: "1"; config: "010101" }
                    ListElement { label: "2"; config: "010110" }
                    ListElement { label: "3"; config: "011001" }
                    ListElement { label: "4"; config: "011010" }
                    ListElement { label: "5"; config: "100101" }
                    ListElement { label: "6"; config: "100110" }
                    ListElement { label: "7"; config: "101001" }
                    ListElement { label: "8"; config: "101010" }
                    ListElement { label: "All"; config: "000000" }
                    ListElement { label: "None"; config: "111111" }
                }
                columns: 8

                Connections {
                    target: RoutineController
                    onSetMultiplexer: {
                        //console.log("Caught setMultiplexer signal, channel: " + label)
                        muxControl.setMuxToLabel(label);
                    }
                }
            }


            RowLayout {
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    text: qsTr("Inputs")
                    Layout.fillWidth: true
                    font.pointSize: Style.heading1.fontSize
                    padding: Style.heading1.padding
                    leftPadding: Style.heading1.paddingLeft
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
        // Note: it appears that a Qt bug causes the minimum window width to be miscalculated at
        // startup, on High DPI displays on Windows. For example: if minimumWidth is 800, and the app is
        // opened on a display with scaling set to 125%, then the window will be resized to (minimum) 1000.
        // (if the window was bigger than 1000, then nothing happens, as expected)
        // So, if you observe unexpected resizing of the window just after launch, this is probably why.
        mainWindow.minimumWidth = Qt.binding(function() { return minimumWidth })
    }
}







/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:14;flowX:"-324";flowY:161}
}
##^##*/
