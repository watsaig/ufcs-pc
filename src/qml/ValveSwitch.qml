import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import org.example.ufcs 1.0

Item {
    property int valveNumber

    Layout.preferredHeight: button.height
    Layout.preferredWidth: button.width

    Button {
        id: button
        text: valveNumber

        height: 50
        width: 1.2*height

        checkable: true
        enabled: Backend.connectionStatus == "Connected"

        onClicked: {
            Backend.setValve(valveNumber, checked);
        }
    }

    ValveSwitchHelper {
        id: helper
        onStateChanged: button.checked = state
    }

    Component.onCompleted: helper.valveNumber = valveNumber

}
