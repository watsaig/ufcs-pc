import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.example.ufcs 1.0

Item {
    property int valveNumber

    Layout.preferredHeight: button.height
    Layout.preferredWidth: button.width

    Button {
        id: button
        text: valveNumber

        height: 60
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
