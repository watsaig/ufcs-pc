import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.example.ufcs 1.0

Item {
    property int valveNumber

    implicitHeight: height
    implicitWidth: Style.valveSwitch.defaultWidth
    height: width * Style.valveSwitch.widthToHeightRatio
    Layout.fillWidth: true
    Layout.maximumWidth: Style.valveSwitch.maximumWidth

    Button {
        id: button
        text: valveNumber

        anchors.fill: parent

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

    Component.onCompleted: Backend.registerValveSwitchHelper(valveNumber, helper)

}
