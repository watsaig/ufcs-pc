import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.example.ufcs 1.0

Item {
    property int pumpNumber

    implicitHeight: button.height
    implicitWidth: button.width

    enabled: Backend.connectionStatus == "Connected"

    Switch {
        id: button
        text: qsTr("Pump ") + pumpNumber
        onToggled: {
            Backend.setPump(pumpNumber, checked);
        }
    }

    PumpSwitchHelper {
        id: helper
        onStateChanged: button.checked = state
    }

    Component.onCompleted: helper.pumpNumber = pumpNumber

}
