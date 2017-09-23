import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import org.example.ufcs 1.0

Item {
    property int valveNumber

    height: button.height
    width: button.width

    Button {
        id: button
        text: valveNumber

        height: 60
        width: 1.2*height

        checkable: true
        onCheckedChanged: {
            helper.state = checked
        }
    }

    ValveSwitchHelper {
        id: helper
        onStateChanged: button.checked = state
    }

    Component.onCompleted: helper.valveNumber = valveNumber

}
