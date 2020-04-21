import QtQuick 2.12

import org.example.ufcs 1.0

Item {
    id: control
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
        y: 0
        width: 800
        height: 600
        fillMode: Image.PreserveAspectFit
        source: "qrc:/res/images/v4_flow_layer.png"
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch1
        x: 271
        y: 155
        width: 260
        height: 35
        valveNumber: 7
        enabled: !editingMode
    }


    GraphicalValveSwitch {
        id: graphicalValveSwitch
        x: 641
        y: 29
        width: 35
        height: 44
        valveNumber: 12
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch2
        x: 271
        y: 234
        width: 260
        height: 35
        valveNumber: 6
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch3
        x: 270
        y: 460
        width: 260
        height: 35
        valveNumber: 5
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch4
        x: 640
        y: 108
        width: 35
        height: 44
        valveNumber: 11
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch5
        x: 640
        y: 191
        width: 35
        height: 44
        valveNumber: 10
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch6
        x: 640
        y: 343
        width: 35
        height: 44
        valveNumber: 9
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch7
        x: 640
        y: 508
        width: 35
        height: 44
        valveNumber: 8
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch8
        x: 126
        y: 108
        width: 35
        height: 44
        valveNumber: 1
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch9
        x: 127
        y: 191
        width: 35
        height: 44
        valveNumber: 2
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch10
        x: 127
        y: 343
        width: 35
        height: 44
        valveNumber: 3
        enabled: !editingMode
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch11
        x: 127
        y: 508
        width: 35
        height: 44
        valveNumber: 4
        enabled: !editingMode
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.8999999761581421;height:480;width:640}
}
##^##*/
