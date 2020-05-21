import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

import org.example.ufcs 1.0

Item {
    id: control
    property int valveNumber

    property color normalColor: "#e60000"
    property color highlightedColor: "#d10000"

    height: 50
    width: 30

    Button {
        id: button
        text: valveNumber
        anchors.fill: parent
        contentItem: Text {
            id: buttonLabel
            text: button.text
            font: button.font
            color: "white"
            //style: Text.Outline
            //styleColor: "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        checkable: true
        checked: false

        onClicked: Backend.setValve(valveNumber, checked);

        background: Rectangle {
            anchors.fill: button

            color: control.normalColor
            opacity: button.checked ? 1 : 0.5

            Ripple {
                clipRadius: 2
                width: button.width
                height: button.height
                pressed: button.pressed
                anchor: button
                active: button.down || button.visualFocus || button.hovered
                color: control.highlightedColor

            }
        }

    }

    ValveSwitchHelper {
        id: helper
        onStateChanged: button.checked = state
    }

    Component.onCompleted: Backend.registerValveSwitchHelper(valveNumber, helper)
}
