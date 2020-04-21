import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.example.ufcs 1.0 // for the Style singleton

Item {
    id: control

    // When True: buttons are disabled; user can edit labels
    property bool editingMode : editingModeSwitch.checked

    property int minimumWidth : inputValveGrid.implicitWidth + 2*Style.view.margin

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Style.view.margin

        Switch {
            id: editingModeSwitch
            text: qsTr("Edit input labels")
        }

        GraphicalChipControl {
            id: graphicalChipControl
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        GridLayout {
            id: inputValveGrid
            Layout.fillWidth: true
            Layout.preferredWidth: parent.width // To do: make this the width of the chip background image, if possible

            //rowSpacing: 5
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
        }
    }
}







/*##^##
Designer {
    D{i:14;flowX:-324;flowY:161}
}
##^##*/
