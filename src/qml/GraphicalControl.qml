import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4

Item {
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
        y: 180
        width: 213
        height: 44
        valveNumber: 7
    }


    GraphicalValveSwitch {
        id: graphicalValveSwitch
        x: 609
        y: 74
        width: 33
        height: 56
        valveNumber: 12
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch2
        x: 294
        y: 244
        width: 213
        height: 44
        valveNumber: 6
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch3
        x: 294
        y: 428
        width: 213
        height: 44
        valveNumber: 5
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch4
        x: 609
        y: 139
        width: 33
        height: 56
        valveNumber: 11
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch5
        x: 609
        y: 206
        width: 33
        height: 56
        valveNumber: 10
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch6
        x: 609
        y: 330
        width: 33
        height: 56
        valveNumber: 9
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch7
        x: 609
        y: 465
        width: 33
        height: 56
        valveNumber: 8
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch8
        x: 170
        y: 139
        width: 33
        height: 56
        valveNumber: 1
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch9
        x: 170
        y: 206
        width: 33
        height: 56
        valveNumber: 2
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch10
        x: 170
        y: 330
        width: 33
        height: 56
        valveNumber: 3
    }

    GraphicalValveSwitch {
        id: graphicalValveSwitch11
        x: 170
        y: 465
        width: 33
        height: 56
        valveNumber: 4
    }


}







/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.25;height:600;width:800}
}
##^##*/
