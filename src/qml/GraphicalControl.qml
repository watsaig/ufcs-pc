import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4

Item {
    id: control

    height: 900
    width: 800

    // When True: buttons are disabled; user can edit labels
    property bool editingMode : editingModeSwitch.checked

    Switch {
        id: editingModeSwitch
        x: 37
        y: 18
        text: qsTr("Edit input labels")
    }

    GridLayout {
        id: gridLayout
        x: 8
        y: 589
        width: 784
        rowSpacing: 5
        columns: 6
        rows: 2

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

    Image {
        id: image
        x: 0
        y: 0
        width: 800
        height: 600
        fillMode: Image.PreserveAspectFit
        source: "qrc:/res/images/v4_flow_layer.png"
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch1
        x: 294
        y: 187
        width: 213
        height: 30
        valveNumber: 7
        enabled: !editingMode
    }


    GraphicalValveSwitch {
        id: graphicalValveSwitch
        x: 609
        y: 80
        width: 33
        height: 44
        valveNumber: 12
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch2
        x: 294
        y: 251
        width: 213
        height: 30
        valveNumber: 6
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch3
        x: 294
        y: 435
        width: 213
        height: 30
        valveNumber: 5
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch4
        x: 609
        y: 145
        width: 33
        height: 44
        valveNumber: 11
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch5
        x: 609
        y: 212
        width: 33
        height: 44
        valveNumber: 10
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch6
        x: 609
        y: 336
        width: 33
        height: 44
        valveNumber: 9
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch7
        x: 609
        y: 471
        width: 33
        height: 44
        valveNumber: 8
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch8
        x: 170
        y: 145
        width: 33
        height: 44
        valveNumber: 1
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch9
        x: 170
        y: 212
        width: 33
        height: 44
        valveNumber: 2
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch10
        x: 170
        y: 336
        width: 33
        height: 44
        valveNumber: 3
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch11
        x: 170
        y: 471
        width: 33
        height: 44
        valveNumber: 4
        enabled: !editingMode
    }




}







/*##^##
Designer {
    D{i:14;flowX:-324;flowY:161}
}
##^##*/
