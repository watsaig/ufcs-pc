import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import org.example.ufcs 1.0

Item {
    property int valveNumber

    height: 50
    width: 30

    Button {
        id: button
        text: valveNumber
        checked: false
        anchors.fill: parent

        checkable: true

        background: Rectangle {
            color: "#e60000"
            opacity: button.checked ? 1 : 0.5
        }

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
