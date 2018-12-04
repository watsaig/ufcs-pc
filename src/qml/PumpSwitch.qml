import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import org.example.ufcs 1.0

Item {
    property int pumpNumber

    Layout.preferredHeight: button.height
    Layout.preferredWidth: button.width

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
