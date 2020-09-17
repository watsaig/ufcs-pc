import QtQuick 2.12

import org.example.ufcs 1.0
import QtQuick.Controls 2.13

Item {
    id: control
    width: 600
    height: 780
    implicitHeight: chipBackground.height
    implicitWidth: chipBackground.width

    onHeightChanged: {
        //console.log("Height changed. WxH: " + width + "x" + height)
        scale = Math.min(height / implicitHeight, width/implicitWidth)
    }
    onWidthChanged : {
        //console.log("Width changed. WxH: " + width + "x" + height)
        scale = Math.min(height / implicitHeight, width/implicitWidth)
    }

    transformOrigin: Item.TopLeft

    Image {
        id: chipBackground
        x: 0
        y: -12
        width: 600
        height: 792
        fillMode: Image.PreserveAspectFit
        source: "qrc:/res/images/v5_flow_layer.png"
    }

    GraphicalValveSwitch {
        x: 128
        y: 713
        width: 115
        height: 44
        customLabel: " (output 1)"
        valveNumber: 1
    }

    GraphicalValveSwitch {
        x: 334
        y: 713
        width: 115
        height: 44
        customLabel: " (output 2)"
        valveNumber: 2
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch19
        x: 230
        y: 250
        width: 117
        height: 35
        valveNumber: 19
    }


    GraphicalValveSwitch {
        id: graphicalValveSwitch21
        x: 147
        y: 87
        width: 35
        height: 44
        valveNumber: 21
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch17
        x: 185
        y: 398
        width: 100
        height: 35
        valveNumber: 17
        customLabel: " (odd)"
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch15
        x: 230
        y: 541
        width: 117
        height: 35
        valveNumber: 15
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch20
        x: 147
        y: 173
        width: 35
        height: 44
        valveNumber: 20
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch18
        x: 147
        y: 318
        width: 35
        height: 44
        valveNumber: 18
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch16
        x: 147
        y: 465
        width: 35
        height: 44
        valveNumber: 16
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch14
        x: 147
        y: 609
        width: 35
        height: 44
        valveNumber: 14
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch11
        x: 295
        y: 398
        width: 100
        height: 35
        valveNumber: 11
        customLabel: " (even)"
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.5}
}
##^##*/
